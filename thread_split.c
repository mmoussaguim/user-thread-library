#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include <sys/queue.h>

#define STACK_SIZE 1024

typedef struct QueueElt{
  Thread *thread;
  STAILQ_ENTRY(QueueElt) next; 
}QueueElt;

/**************************************************/
/***************** LES GLOBALES *******************/
/**************************************************/
//Identifiant de référence pour les nouveaux threads
int id_ref = 0;

//File d'attente de threads prêts
STAILQ_HEAD(ma_fifo, QueueElt) runqueue;
			       STAILQ_HEAD(ma_fifo2, QueueElt) waitqueue;
							      int queue_is_init = 0;			    

//Pointeur du thread en exécution
Thread current_thread; // à initialiser ?
Thread* running_thread = &current_thread;

/**************************************************/
/***************** LES FONCTIONS ******************/
/**************************************************/

#include "thread_self.c"

#include "thread_create.c"

#include "thread_yield.c"

#include "thread_join.c"

#include "thread_exit.c"
