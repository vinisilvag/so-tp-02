#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_yield2(void)
{
  yield2();
  return 0; // provavelmente nao eh alcancado
}

int
sys_wait3(void)
{
  int *priority;
  int *retime;
  int *rutime;

  argptr(0, (char**)&priority, sizeof(int));
  argptr(1, (char**)&retime, sizeof(int));
  argptr(2, (char**)&rutime, sizeof(int));

  return wait3(priority, retime, rutime);
}

int
sys_wait2(void)
{
  int *retime;
  int *rutime;
  int *stime;

  argptr(0, (char**)&retime, sizeof(int));
  argptr(1, (char**)&rutime, sizeof(int));
  argptr(2, (char**)&stime, sizeof(int));

  return wait2(retime, rutime, stime);
}

int
sys_set_prio(void)
{
  int priority, pid;

  argint(0, &priority);
  argint(1, &pid);

  return set_prio(priority, pid);
}

int
sys_change_prio(void)
{
  int priority;
  argint(0, &priority);

  return change_prio(priority);
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
