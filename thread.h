#include <ucontext.h>
#include "interface.h"
#include <valgrind/valgrind.h>

typedef struct Thread{
  ucontext_t uc;
  thread_t father;
  int is_dead;
  void *retval;
  int vlg_id;
} Thread;



void init(void) __attribute__((constructor));
void end(void) __attribute__((destructor));
