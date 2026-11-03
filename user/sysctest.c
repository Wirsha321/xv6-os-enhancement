#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int before = getsyscallcount();

  getpid();
  uptime();

  int after = getsyscallcount();

  printf("System call count before: %d\n", before);
  printf("System call count after: %d\n", after);

  if(after - before == 3)
    printf("System call counter test PASSED\n");
  else
    printf("System call counter test FAILED\n");

  exit(0);
}
