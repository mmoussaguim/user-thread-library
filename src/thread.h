#ifndef THREAD_H
#define THREAD_H

#include <ucontext.h>
#include "interface.h"
#include <valgrind/valgrind.h>
#include <sys/queue.h>

typedef struct Thread{
  ucontext_t* uc;
  thread_t father;
  int is_dead;
  void *retval;
  int vlg_id;
  int priority; // 0 <= priority <= 20
  STAILQ_ENTRY(Thread) next; //for runqueue
} Thread;



void init(void) __attribute__((constructor));
void end(void) __attribute__((destructor));
int run_other_thread(Thread * old_thread);
int preemptime(Thread * thread);
void preempt(int signum);
int thread_setschedprio(Thread * thread, int prio);
void insert_runqueue(Thread *thread);
void mask_signal(int signum);
void unmask_signal(int signum);
void mem_use();


#endif
