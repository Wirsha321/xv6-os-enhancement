#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  setpriority(20);
  printf("Current priority = %d\n", getpriority());
  exit(0);
}
