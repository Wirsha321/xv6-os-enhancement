#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  struct sleepstats info;
  for(int i = 0; i < 1000000; i++) { }

  if(getsleepstats(&info) < 0) {
    printf("Sleep statistics failed\n");
    exit(1);
  }

  printf("Sleep count = %d\n", info.sleeps);
  printf("Wakeup count = %d\n", info.wakeups);
  exit(0);
}
