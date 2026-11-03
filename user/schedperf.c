#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
run_test(int policy)
{
  int priorities[3] = {10, 50, 90};
  int tickets[3] = {10, 20, 30};
  int started = 0;

  if(setschedpolicy(policy) < 0){
    printf("Could not set scheduler policy\n");
    return;
  }

  printf("\n=== %s Scheduler Test ===\n",
         policy == 0 ? "PRIORITY" : "LOTTERY");

  for(int i = 0; i < 3; i++){
    int pid = fork();

    if(pid < 0){
      printf("Fork failed\n");
      break;
    }

    if(pid == 0){
      if(policy == 0)
        setpriority(priorities[i]);
      else
        settickets(tickets[i]);

      for(volatile int j = 0; j < 20000000; j++)
        ;

      printf("%s PID %d config %d CPU ticks %d\n",
             policy == 0 ? "PRIORITY" : "LOTTERY",
             getpid(),
             policy == 0 ? priorities[i] : tickets[i],
             getcputicks());
      exit(0);
    }

    started++;
  }

  for(int i = 0; i < started; i++)
    wait(0);
}

int
main(void)
{
  printf("Scheduler Performance Benchmark\n");

  run_test(0);
  run_test(1);

  setschedpolicy(0);

  printf("\nScheduler performance test completed\n");
  exit(0);
}
