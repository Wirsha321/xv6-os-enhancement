#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int p[2];
  char c;

  pipe(p);

  if(fork() == 0) {
    close(p[1]);
    setpriority(10);
    read(p[0], &c, 1);
    for(volatile int i = 0; i < 10000000; i++);
    printf("HIGH priority 10 finished\n");
    exit(0);
  }

  if(fork() == 0) {
    close(p[1]);
    setpriority(80);
    read(p[0], &c, 1);
    for(volatile int i = 0; i < 10000000; i++);
    printf("LOW priority 80 finished\n");
    exit(0);
  }

  close(p[0]);
  write(p[1], "ab", 2);
  close(p[1]);
  wait(0);
  wait(0);
  exit(0);
}
