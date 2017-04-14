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
			       int queue_is_init = 0;			    

//Pointeur du thread en exécution
Thread current_thread; // à initialiser ?
Thread* running_thread = &current_thread;

/**************************************************/
/***************** LES FONCTIONS ******************/
/**************************************************/

extern thread_t thread_self(void){
  return running_thread;
}

extern int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg){
  //Préparer le contexte
  ucontext_t uc;
  getcontext(&uc); //TODO : attraper l'erreur 
  uc.uc_stack.ss_size = 64*STACK_SIZE;
  uc.uc_stack.ss_sp = malloc(uc.uc_stack.ss_size);
  makecontext(&uc, (void(*)(void))*func, 1, funcarg);

  //Créer un Thread
  Thread *new_th = malloc(sizeof(Thread));
  new_th->id = id_ref++;
  new_th->uc = uc;
  new_th->father = NULL;

  //Créer un QueueElt
  QueueElt *new_elt = malloc(sizeof(QueueElt));
  new_elt->thread = new_th;
  
  //L'ajouter à la runqueue
  if(!queue_is_init){
    STAILQ_INIT(&runqueue);
    queue_is_init = 1;
  }
  STAILQ_INSERT_TAIL(&runqueue, new_elt, next);
  
  return 0;
}

extern int thread_yield(void){
  Thread * old_thread = running_thread;
  // Le thread courant va dans la runqueue 
  QueueElt *run_elt = malloc(sizeof(QueueElt));
  run_elt->thread = thread_self();
  STAILQ_INSERT_TAIL(&runqueue, run_elt, next);
  
  // Si l'ajout ne s'est pas bien passé, quitter
  if(STAILQ_EMPTY(&runqueue))
    return 1;
  // Passer le premier thread de la runqueue en running
  run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
  STAILQ_REMOVE_HEAD(&runqueue, next); //ERREUR type incomplet
  running_thread = run_elt->thread;
  free(run_elt);

  // Recuperer le contexte du nouveau thread courant
  // Et stocker le contexte courrant dans l'ancien thread courant
  swapcontext(&(old_thread->uc),&(running_thread->uc));
  
  return 0;
}

/* attendre la fin d'exécution d'un thread.
 * la valeur renvoyée par le thread est placée dans *retval.
 * si retval est NULL, la valeur de retour est ignorée.
 */
extern int thread_join(thread_t thread, void **retval){
  //le thread courant est passé en father de 'thread', et quitte l'état running
  //run le premier de la fifo
  // Attention gérer le cas ou le htread a deja terminé
  return 0;
}

/* terminer le thread courant en renvoyant la valeur de retour retval.
 * cette fonction ne retourne jamais.
 *
 * L'attribut noreturn aide le compilateur à optimiser le code de
 * l'application (élimination de code mort). Attention à ne pas mettre
 * cet attribut dans votre interface tant que votre thread_exit()
 * n'est pas correctement implémenté (il ne doit jamais retourner).
 */
extern void thread_exit(void *retval){
  //écrit dans retval avant de free
	
	Thread *father = current_thread.father;

	kill(getpid(thread_self()),SIGKILL);	

	//mettre le father à la fin de la runqueue
	QueueElt *run_elt = malloc(sizeof(QueueElt));
  	run_elt->thread = father;
	STAILQ_INSERT_TAIL(&runqueue, run_elt, next);

  	//run le premier de la fifo
	
	run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
	STAILQ_REMOVE_HEAD(&runqueue, next); //ERREUR type incomplet
  	running_thread = run_elt->thread;
  	free(run_elt);
}
