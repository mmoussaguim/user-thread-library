#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

#define MUTEX_AVAILABLE 1
#define MUTEX_LOCKED 0
#define MUTEX_DESTROYED -1


/* Returns 0 if success, else -1 */
int thread_mutex_init(thread_mutex_t *mutex){
  if(mutex != NULL){
    mutex->state = MUTEX_AVAILABLE;
    mutex->owner = NULL;
    return 0;
  }
  return -1;
}

/* Returns 0 if success, else -1 
  * Trying to destroy a locked thread has undefined behavior
  */
int thread_mutex_destroy(thread_mutex_t *mutex){
  if(mutex != NULL){
    mutex->state = MUTEX_DESTROYED;
    mutex->owner = NULL;
    return 0;
  }
  return -1;
}

/* 
 * Returns 0 when locked, nothing else.
 */
int thread_mutex_lock(thread_mutex_t *mutex){
  if(mutex->state == MUTEX_DESTROYED)
      return -1;
  int old_state = __sync_lock_test_and_set(&(mutex->state), MUTEX_LOCKED);
  while(old_state != MUTEX_AVAILABLE){
    if(old_state == MUTEX_DESTROYED)
      return -1;
    thread_yield();
    old_state = __sync_lock_test_and_set(&(mutex->state), MUTEX_LOCKED);
  }
  mutex->owner = thread_self();
  return 0;
}

/*
 * Unlocks mutex only if not already destroyed or owned by sb else
 * Returns 0 if success, else -1.
 */
int thread_mutex_unlock(thread_mutex_t *mutex){
  if(mutex->state == MUTEX_DESTROYED || mutex->owner != thread_self())
    return -1;
  mutex->state = MUTEX_AVAILABLE;
  return 0;
}



