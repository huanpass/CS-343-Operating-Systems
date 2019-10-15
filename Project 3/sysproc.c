#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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

int
sys_clone(void) {
  void (*fcn)(void*);
  void *arg, *stack;

  // arg err checking for clone sys call
  if (argptr(0, (char**)&fcn, sizeof(void*)) < 0) {
    return -1;
  }

  if (argptr(1, (char**)&arg, sizeof(void*)) < 0) {
    return -1;
  }

  if (argptr(2, (char**)&stack, PGSIZE) < 0) {
    return -1;
  }

  return clone(fcn, arg, stack);

}

int
sys_join(void) {
  int pid;
  
  // arg error checking for join sys call
  if (argint(0, &pid) < 0) {
    return -1;
  }

  return join(pid);
}

int
sys_csleep(void) {
  cond_t* conditionVariable;
  lock_t* lock;

  // arg error checking for cv_wait sys call
  if (argptr(0, (char**)&conditionVariable, sizeof(cond_t*)) < 0) {
    return -1;
  }

  if (argptr(1, (char**)&lock, sizeof(lock_t*)) < 0) {
    return -1;
  }

  csleep(conditionVariable, lock);

  // return 0 on success
  return 0;
}

int
sys_cwake(void) {
  cond_t* conditionVariable;

  // arg error checking for cv_signal sys call
  if (argptr(0, (char**)&conditionVariable, sizeof(cond_t*)) < 0) {
    return -1;
  }

  cwake(conditionVariable);

  // return 0 on success
  return 0;
}

int
sys_ustack(void) {
  int pid;

  // arg error checking for ustack sys call
  if (argint(0, &pid) < 0) {
    return -1;
  }

  return ustack(pid);
}
