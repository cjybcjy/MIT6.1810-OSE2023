#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


void run_cmd(char *cmd[]) {
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "cannot fork\n");
        exit(1);
    }       

    if (pid == 0) {
        if (exec(cmd[0], cmd) < 0) {
            fprintf(2, "cannot exec\n");
            exit(1);
        }
        exit(0);
    } else {
        wait(0);
    }
    
}


int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        fprintf(2, "Usage: xargs [options] [command [initial-arguments]]\n");
        exit(1);
    }

    if (argc > MAXARG - 1) {
        fprintf(2, "too many arguments: MAXARG %d\n", MAXARG - 1);
        exit(0);
    }

    //buf to store command line
    char buf[512];
    char *p = buf;
    int n;
    
    // Prepare the command array from the arguments
    char *cmd[MAXARG];
    for (int i = 1; i < argc; i++) {
        cmd[i - 1] = argv[i];
    }

    //fork & exec to invoke the command on each line of input 
    while ((n = read(0, p, 1)) > 0) {
        if (*p == '\n') {//read a argument
            *p = 0;
            cmd[argc - 1] = buf;
            cmd[argc] = 0;//end argument
            run_cmd(cmd);
            p = buf;//refresh buf
        } else {
            p++;
            //check buf overflow
            if (p >= buf + sizeof(buf)) {
                fprintf(2, "xargs: input line is too long");
                exit(1);
            }
        }
    }

    if (n < 0) {
        fprintf(2, "xargs: read argument error\n");
        exit(1);
    }

    exit(0);//n = 0, end of cmd array
}

