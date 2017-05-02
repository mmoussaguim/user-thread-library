#include <ucontext.h>
#include "interface.h"
#include <valgrind/valgrind.h>

typedef struct Thread{
  ucontext_t uc;
  thread_t father;
  int is_dead;
  void *retval;
  int vlg_id;
  int priority;
} Thread;



void init(void) __attribute__((constructor));
void end(void) __attribute__((destructor));
int run_other_thread(Thread * old_thread);
void preempt(int signum);
int thread_setschedprio(Thread * thread, int prio);
