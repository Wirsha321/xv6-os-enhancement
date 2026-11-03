#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int values[3] = {10, 20, 30};

  printf("Starting lottery scheduler test\\n");

  if (setschedpolicy(1) < 0) {
    printf("Could not enable lottery scheduler\\n");
    exit(1);
  }

  for (int i = 0; i < 3; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("Fork failed\\n");
      setschedpolicy(0);
      exit(1);
    }
    if (pid == 0) {
      if (settickets(values[i]) < 0)
        exit(1);

      volatile int count;
      for (count = 0; count < 2000000; count++)
        ;
      printf("Child finished with %d tickets\\n", values[i]);
      exit(0);
    }
  }

  for (int i = 0; i < 3; i++)
    wait(0);

  setschedpolicy(0);
  printf("Lottery scheduler test completed\\n");
  exit(0);
}
