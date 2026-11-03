#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int ready[2], block[2], pid, i;
  char c = 'R';
  struct sleepstats info;

  pipe(ready);
  pipe(block);

  pid = fork();

  if(pid == 0) {
    close(ready[0]);
    close(block[1]);
    write(ready[1], &c, 1);
    read(block[0], &c, 1);
    getsleepstats(&info);
    printf("Child sleep count = %d\n", info.sleeps);
    printf("Child wakeup count = %d\n", info.wakeups);
    exit(0);
  }

  close(ready[1]);
  close(block[0]);

  read(ready[0], &c, 1);

  for(i = 0; i < 1000000; i++)
    ;

  kill(pid);
  wait(0);

  printf("Killed sleeping child test completed\n");
  exit(0);
}
