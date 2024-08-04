#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

#define LAB_PGTBL 1

#ifdef LAB_PGTBL
//reports which pages have been accessed
int
sys_pgaccess(void)
{
  uint64 start_vapg;
  int  npg;
  uint64 maskbuf;
  uint mask = 0;
  
  argaddr(0, &start_vapg);
  argint(1, &npg);
  argaddr(2, &maskbuf); 
  pagetable_t u_pt = myproc()->pagetable;

  if(npg > 32) {//4B = 32b
        return -1;
  }



  for (int i = 0; i < npg; i++) {
      pte_t *pt = walk(u_pt, start_vapg + i * PGSIZE, 0);
      if ((*pt & PTE_A) && (*pt & PTE_V)) {
            mask |= (1 << i);
            //clear PTE_A after checking if it is set
            *pt ^= PTE_A;
      }
    }


    //获取 mask 的地址并将其转换为 char*
     if (copyout(u_pt, maskbuf, (char*)&mask, 4) != 0) {
        return -1;
     }
     
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
