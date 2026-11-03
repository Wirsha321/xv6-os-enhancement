#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  struct meminfo info;

  if(getmeminfo(&info) < 0) {
    printf("Memory info syscall failed\n");
    exit(1);
  }

  printf("Process memory: %d bytes\n", (int)info.process_mem);
  printf("Free physical memory: %d bytes\n", (int)info.free_mem);

  if(info.process_mem > 0 && info.free_mem > 0)
    printf("Memory statistics test PASSED\n");
  else
    printf("Memory statistics test FAILED\n");

  exit(0);
}
