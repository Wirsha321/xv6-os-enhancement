#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(void)
{
  struct pinfo infos[NPROC];
  int count, i, s;
  char *states[] = {"unused", "used", "sleeping", "runnable", "running", "zombie"};

  count = getproctree(infos, NPROC);
  if(count < 0) {
    printf("Process tree failed\n");
    exit(1);
  }

  printf("\nPID   PPID  STATE      NAME\n");
  for(i = 0; i < count; i++) {
    s = infos[i].state;
    if(s < 0 || s > 5)
      s = 0;
    printf("%d     %d     %s    %s\n",
           infos[i].pid, infos[i].ppid, states[s], infos[i].name);
  }
  exit(0);
}
