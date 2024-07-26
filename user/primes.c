#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


//for each prime use pipe to filte the num left
void prime_filter( int left_read_fd) {
    int prime;
    if (read(left_read_fd, &prime, sizeof(prime)) <= 0){//finish
        close(left_read_fd);
        return;
    }
    printf("prime %d\n", prime);

    int p[2];
    if (pipe(p) == -1) {
        printf("pipe error\n");
        close(left_read_fd);
        exit(1);
    }

    int pid = fork();
    if (pid == -1) {
        printf("fork error\n");
        close(left_read_fd);
        close(p[0]);
        close(p[1]);
        exit(1);
    }

    if (pid != 0) {//parent process
        // read number from read_end && judge if prime then write to p[1]
        close(p[0]);
        int num;
        while (read(left_read_fd, &num, sizeof(num)) > 0) {
            if (num % prime != 0) {
                write(p[1], &num, sizeof(num));
            }
        }
        close(left_read_fd);
        close(p[1]);
        wait(0);
        exit(0);
    } else {//child process read from p[0]
        close(p[1]);
        // recursive prime_filter;
        prime_filter(p[0]);
        close(p[0]);
        exit(0);
    }    
}


int main() {

    int p[2];
    if (pipe(p) == -1) {
        printf("pipe error\n");
        exit(1);
    }

    int pid = fork();
    if (pid == -1) {
        printf("fork error\n");
        close(p[0]);
        close(p[1]);
        exit(1);
    }

    //parent process read_end pass num to pipe
    //child process use prime_filter
    if (pid != 0) {
        close(p[0]);
        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
        exit(0);
    } else {
        close(p[1]);//child process close stdout
        prime_filter(p[0]);
        close(p[0]);
        exit(0);
    }
}