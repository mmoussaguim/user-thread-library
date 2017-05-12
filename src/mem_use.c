#include "mem_use.h"

void mem_use(){
  pid_t pid = getpid();
  char test[1024];
  sprintf(test,"cat /proc/%d/status | awk '/VmSize/ {print $2}' >> mem_use.txt",pid);
  system(test);
}
