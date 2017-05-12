#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

Thread* running_thread;

/* Returns 0 if success, else -1 */
int thread_mutex_init(thread_mutex_t *mutex){
  if(mutex != NULL){
    mutex->is_destroyed = 0;
    mutex->owner = NULL;
    STAILQ_INIT(&(mutex->waitqueue));
    return 0;
  }
  return -1;
}

/* Returns 0 if success, else -1 
  * Trying to destroy a locked thread has undefined behavior
  */
int thread_mutex_destroy(thread_mutex_t *mutex){
  if(mutex != NULL){
    mutex->is_destroyed = 1;
    mutex->owner = NULL;
    //vider la waitqueue? 
    return 0;
  }
  return -1;
}

/* 
 * Returns 0 when locked, nothing else.
 */
int thread_mutex_lock(thread_mutex_t *mutex){
  if(mutex->is_destroyed)
      return -1;
  //Mask signals to avoid being preempted
  mask_signal(SIGALRM);
  Thread* old_owner = __sync_lock_test_and_set(&(mutex->owner), running_thread);
  if(old_owner != NULL){ //mutex wasn't available
    mutex->owner = old_owner;
    STAILQ_INSERT_TAIL(&(mutex->waitqueue), running_thread, next);
    run_other_thread(running_thread);
  }
  unmask_signal(SIGALRM);
  return 0;
}

/*
 * Unlocks mutex only if not already destroyed or owned by sb else
 * Returns 0 if success, else -1.
 */
int thread_mutex_unlock(thread_mutex_t *mutex){
  if(mutex->is_destroyed || mutex->owner != running_thread)
    return -1;
  mask_signal(SIGALRM);
  if(!STAILQ_EMPTY(&(mutex->waitqueue))){ //there is a thread waiting for this mutex
    Thread* new_owner = STAILQ_FIRST(&(mutex->waitqueue));
    STAILQ_REMOVE_FIRST(&(mutex->waitqueue));
    mutex->owner = new_owner;
    STAILQ_INSERT_TAIL(&runqueue, new_owner, next);
  }
  else{
    mutex->owner = NULL;
  }
  unmask_signal(SIGALRM);
  return 0;
}



