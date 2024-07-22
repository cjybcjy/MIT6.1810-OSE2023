#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main() {
      int pp1[2], pp2[2];
      pipe(pp1);
      pipe(pp2);
      char buf1 = ' ';
      char buf2 = ' ';

      if (fork () != 0) {//parent process
            write(pp1[1], "!", 1);
            wait(0);
            read(pp2[0], &buf1, 1);
            printf("%d: received pong\n", getpid());
            exit(0);
      } else {
            read(pp1[0], &buf1, 1);
            printf("%d: received ping\n", getpid());
            write(pp2[1], &buf2, 1);
            exit(0);
      }
}