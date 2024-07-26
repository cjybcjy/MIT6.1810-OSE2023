#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"



char* fmtname(char *path) {//从完整的文件路径中分离出单独的文件名。           
    char *p;

    //Find first character after last slash
    for (p = path + strlen(path); p >= path && *p != '/'; p--) { ;}
    p++;
    
   return p;
}

void find(char *path, char *filename) {
    
    //文件路径长度常常有一个定义的最大值
    char buf[512], *p;
    struct stat st;
    struct dirent de;
    int fd; 
    //open uncorrectly
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    //couldn't get fd's stat
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
        case T_DEVICE:
        case T_FILE:
            if (strcmp(fmtname(path), filename) == 0) {
                    printf("%s\n", path);
            }
            break;
        case T_DIR:
            if (strlen(path) + 1 + strlen(filename) + 1 > sizeof(buf)) {
                printf("find: path is too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
        //read 返回值等于 sizeof(de)，则循环继续，表明还有更多的目录项可以读取。
        //如果返回值小于 sizeof(de)（包括返回0），这通常意味着已经达到了目录的末尾或发生了读取错误。
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                //如果读取的目录项的 inode 号是 0，表示这是一个未使用的目录项，跳过，继续搜索
                if (de.inum == 0) continue;
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;//跳过
                int name_len = strlen(de.name);
                memmove(p, de.name, name_len);
                //确保在 p 指向的字符串后添加一个 null 终止符（即字符 '\0'），这是 C 语言字符串的标准结束符。
                p[name_len] = 0;
                //递归寻找, DFS
                find(buf, filename);
            }
            break;      
    }
    close(fd);
}


int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(2, "Usage: find <path> <filename>\n");
        exit(1);
    }
    
    find(argv[1], argv[2]);
    exit(0);
}