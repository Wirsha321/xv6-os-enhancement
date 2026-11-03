#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  volatile int i;
  for(i = 0; i < 500000000; i++)
    ;
  printf("CPU ticks used = %d\n", getcputicks());
  exit(0);
}
