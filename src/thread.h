#ifndef __THREAD_H__
#define __THREAD_H__

#include <ucontext.h>
#include "interface.h"
#include <valgrind/valgrind.h>
#include <sys/queue.h>
#include <sys/time.h>

typedef struct Thread{
  ucontext_t* uc;
  thread_t father;
  int is_dead;
  void *retval;
  int vlg_id;
  int priority; // 0 <= priority <= 20
  STAILQ_ENTRY(Thread) next; //for runqueue
#ifdef TESTTEMPS
  int nb_exec; // Pour mesurer le nombre d'exécution dans les tests
  struct timeval current_tps_exec;// Pour mesurer le temps d'éxécution des thread
  long long int total_tps_exec;
#endif
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
