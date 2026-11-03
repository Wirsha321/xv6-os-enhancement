#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  struct procinfo info;

  if(getprocinfo(&info) < 0) {
    printf("Process info failed\n");
    exit(1);
  }

  printf("Process ID = %d\n", info.pid);
  printf("Process priority = %d\n", info.priority);
  exit(0);
}
