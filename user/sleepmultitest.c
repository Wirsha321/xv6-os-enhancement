#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int fd[2], ready[2];
  int pid, i;
  char c = 'A';
  struct sleepstats info;

  for(i = 0; i < 3; i++) {
    if(pipe(fd) < 0 || pipe(ready) < 0)
      exit(1);

    pid = fork();

    if(pid < 0)
      exit(1);

    if(pid == 0) {
      close(fd[1]);
      close(ready[0]);

      write(ready[1], &c, 1);
      close(ready[1]);

      read(fd[0], &c, 1);

      if(getsleepstats(&info) == 0)
        printf("Child %d: sleeps=%d wakeups=%d\n",
               getpid(), info.sleeps, info.wakeups);

      close(fd[0]);
      exit(0);
    }

    close(fd[0]);
    close(ready[1]);

    read(ready[0], &c, 1);
    close(ready[0]);

    for(volatile int j = 0; j < 10000000; j++)
      ;

    write(fd[1], &c, 1);
    close(fd[1]);

    wait(0);
  }

  printf("Three child wakeup tests completed\n");
  exit(0);
}
