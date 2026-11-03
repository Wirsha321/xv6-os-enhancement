#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int result = setpriority(10);
  printf("setpriority result = %d\n", result);
  exit(0);
}
