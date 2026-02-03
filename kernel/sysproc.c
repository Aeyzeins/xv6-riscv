#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "procinfo.h"
#include "vm.h"

extern struct proc proc[NPROC];
extern struct spinlock wait_lock;


uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
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
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
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

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
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

// Return the code of our OS course
uint64
sys_getcourseno(void)
{
    return 3221;
}

//Project 1:
//sys_getprocs: copy process info into user-defined array
uint64
sys_getprocs(void)
{
  uint64 uaddr;                       //user address off buffer where kernsel should write
  int max;                            //max number of entries that the user wants the kernel to fill
  struct proc *p;                     //pointer loop for kernel process table   
  struct procinfo info;               //temp kernel-side struct that holds one process’s info before copying it out to user space   
  struct proc *curproc = myproc();    //current process (caller), used to get its pagetable for copyout()
  int count = 0;                      //number of procinfo structs copied into user space 
  
  argaddr(0, &uaddr);  //get user buffer address
  argint(1, &max);     //get 2nd syscall argument, then store it in max

  if (max < 0) {
    return -1; //returns error if the entry count is negative or zero
  }

  //Now we are going to loop through every process in the process table
  //and copy its info into the user buffer until we filled max entries.
  for(p = proc; p < &proc[NPROC]; p++) {
    int has_entry = 0; //flag to indicate if we added an entry for this process

    //Lock the process to safely read its state, pid, ppid, sz, and name from procinfo.c
    acquire(&wait_lock);
    acquire(&p->lock);

    //We will have it copy only the real processes, not the UNUSED ones.
    if (p->state != UNUSED) {
      info.pid = p->pid;               //Process ID
      info.ppid = (p->parent) ? p->parent->pid : 0; //Parent Process ID
      info.state = p->state;           //Process state
      info.sz = p->sz;                 //Size of process memory in bytes

      /*
      copies the process name from the kernel’s proc struct 
      into the temporary procinfo struct so it can later be 
      copied out to user space.
      */
      memmove(info.name, p->name, sizeof(info.name)); 
      has_entry = 1; //Set flag to indicate we have an entry to copy
    }

    //Release locks after reading process info
    release(&p->lock);
    release(&wait_lock);

    if(has_entry) {
      //copy the info record into user's buffer at the correct offset, otherwise return -1 on failure
      if (copyout(curproc->pagetable, uaddr + count * sizeof(info),
        (char *)&info, sizeof(info)) < 0){
        return -1;
      }
      count++;  //increment count of the next entry.
    }


  
  }

  return count; //return the number of process entries copied into user space
}