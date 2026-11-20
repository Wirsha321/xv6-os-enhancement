#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
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

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
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
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep_prepare(&ticks);
    release(&tickslock);
    sleep();
    acquire(&tickslock);
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

uint64
sys_getproctree(void)
{
  uint64 addr;
  int max, count;
  struct pinfo infos[NPROC];
  struct proc *p = myproc();

  argaddr(0, &addr);
  argint(1, &max);
  if(max <= 0)
    return -1;
  if(max > NPROC)
    max = NPROC;

  count = getproctree(infos, max);
  if(copyout(p->pagetable, p->sz, addr,
             (char *)infos, count * sizeof(struct pinfo)) < 0)
    return -1;
  return count;
}

extern int schedpolicy;
extern struct spinlock sched_lock;

uint64
sys_setschedpolicy(void)
{
  int policy;

  argint(0, &policy);

  if(policy != 0 && policy != 1)
    return -1;

  acquire(&sched_lock);
  schedpolicy = policy;
  release(&sched_lock);

  return 0;
}

uint64
sys_setpriority(void)
{
  int priority;
  argint(0, &priority);

  if(priority < 0 || priority > 100)
    return -1;

  struct proc *p = myproc();
  acquire(&p->lock);
  p->priority = priority;
  release(&p->lock);

  return 0;
}

uint64
sys_getpriority(void)
{
  struct proc *p = myproc();
  int priority;

  acquire(&p->lock);
  priority = p->priority;
  release(&p->lock);

  return priority;
}

uint64
sys_settickets(void)
{
  int tickets;
  argint(0, &tickets);

  if(tickets <= 0)
    return -1;

  struct proc *p = myproc();
  acquire(&p->lock);
  p->tickets = tickets;
  release(&p->lock);

  return 0;
}

uint64
sys_getcputicks(void)
{
  struct proc *p = myproc();
  uint64 count;

  acquire(&p->lock);
  count = p->cputicks;
  release(&p->lock);

  return count;
}


// Return sleep and wakeup statistics for the current process.
uint64
sys_getsleepstats(void)
{
  uint64 addr;
  struct proc *p = myproc();
  struct {
    int sleeps;
    int wakeups;
  } stats;

  argaddr(0, &addr);

  acquire(&p->lock);
  stats.sleeps = p->sleeps;
  stats.wakeups = p->wakeups;
  release(&p->lock);

  if(copyout(p->pagetable, p->sz, addr,
             (char *)&stats, sizeof(stats)) < 0)
    return -1;

  return 0;
}


// Return memory statistics for the current process.
uint64
sys_getmeminfo(void)
{
  uint64 addr;
  struct proc *p = myproc();
  struct {
    uint64 process_mem;
    uint64 free_mem;
  } info;

  argaddr(0, &addr);

  acquire(&p->lock);
  info.process_mem = p->sz;
  release(&p->lock);

  info.free_mem = freemem();

  if(copyout(p->pagetable, p->sz, addr,
             (char *)&info, sizeof(info)) < 0)
    return -1;

  return 0;
}


// Return process information for the current process.
uint64
sys_getprocinfo(void)
{
  uint64 addr;
  struct proc *p = myproc();
  struct {
    int pid;
    int priority;
  } info;

  argaddr(0, &addr);

  acquire(&p->lock);
  info.pid = p->pid;
  info.priority = p->priority;
  release(&p->lock);

  if(copyout(p->pagetable, p->sz, addr,
             (char *)&info, sizeof(info)) < 0)
    return -1;

  return 0;
}

// Return the system call count for the current process.
uint64
sys_getsyscallcount(void)
{
  struct proc *p = myproc();
  return p->syscall_count;
}
