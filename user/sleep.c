
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"//依赖上面两个头文件



int main (int argc, char *argv[]) {//等同 char**一个指向字符指针的指针,每个参数都是一个字符串
    
    if (argc < 2) {
        fprintf(2, "usage: sleep time\n");
        exit(1);
    }
    //argv 是一个字符串数组，包含所有的命令行参数，第一个元素是程序的名称，之后的元素是实际的参数


    sleep(atoi(argv[1]));
    exit(0);
}