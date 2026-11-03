#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pid;
  int status;
  struct sleepstats info;

  pid = fork();
  if(pid < 0) {
    printf("Fork failed\n");
    exit(1);
  }

  if(pid == 0) {
    exit(0);
  }

  wait(&status);

  if(getsleepstats(&info) < 0) {
    printf("Statistics failed\n");
    exit(1);
  }

  printf("Parent sleep count = %d\n", info.sleeps);
  printf("Parent wakeup count = %d\n", info.wakeups);
  printf("Parent-child test completed\n");
  exit(0);
}
