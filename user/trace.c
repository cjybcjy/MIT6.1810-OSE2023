#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];
//检查命令行参数的数量是否少于 3，或者第二个参数是否不是一个数字
  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
    fprintf(2, "Usage: %s mask command\n", argv[0]);
    exit(1);
  }
//调用 trace 函数设置跟踪掩码
  if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
  }
//提取命令行参数，准备执行新的程序
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }
  //执行新命令,exec 系统调用用于将当前进程的映像替换为新的程序。尽管进程的内存和代码会被替换，但某些进程属性
  exec(nargv[0], nargv);
  exit(0);
}
