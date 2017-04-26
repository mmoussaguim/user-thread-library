#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include <sys/queue.h>
#include <ucontext.h> /* ne compile pas avec -std=c89 ou -std=c99 */

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


//Pointeur du thread en exécution
Thread current_thread; // à initialiser ?
Thread* running_thread = &current_thread;





/**************************************************/
/***************** LES FONCTIONS ******************/
/**************************************************/

extern thread_t thread_self(void){
  //printf("--TEST-- self\n");
  return running_thread;
}


// fonction englobante pour connnaitre la fin d'execution du thread
void *tmp(void* (*func)(void*), void *arg){
  void * retval = func(arg);
  if (running_thread->state != dead)
    thread_exit(retval);
  return retval;
  }



extern int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg){

      
  //Préparer le contexte
  static ucontext_t uc;
  getcontext(&uc); //TODO : attraper l'erreur 
  uc.uc_stack.ss_size = 64*STACK_SIZE;
  uc.uc_stack.ss_sp = malloc(uc.uc_stack.ss_size);

    
  makecontext(&uc, (void(*)(void))*tmp, 2,func, funcarg);
  uc.uc_link = NULL; // ?
  
  //Créer un Thread
  Thread *new_th = malloc(sizeof(Thread));
  new_th->id = id_ref++;
  new_th->uc = uc;
  new_th->father = NULL;
  new_th->retval = NULL;
  new_th->state = ready;

int valgrind_stackid = VALGRIND_STACK_REGISTER(uc.uc_stack.ss_sp,
                                               uc.uc_stack.ss_sp + uc.uc_stack.ss_size);
 new_th->vlg_id = valgrind_stackid;

  //Créer un QueueElt
  QueueElt *new_elt = malloc(sizeof(QueueElt));
  new_elt->thread = new_th;
  
  //L'ajouter à la runqueue
  STAILQ_INSERT_TAIL(&runqueue, new_elt, next);
  *newthread = new_th;
  return 0;
}

extern int thread_yield(void){
  if(STAILQ_EMPTY(&runqueue))
    return 0;
    
  Thread * old_thread = running_thread;
  old_thread->state = ready;
  QueueElt *run_elt = malloc(sizeof(QueueElt));
  run_elt->thread = thread_self();

  STAILQ_INSERT_TAIL(&runqueue, run_elt, next); 

  // Si l'ajout ne s'est pas bien passé, quitter
  if(STAILQ_EMPTY(&runqueue))
    return 1;
  // Passer le premier thread de la runqueue en running
  run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
  running_thread = run_elt->thread;
  STAILQ_REMOVE_HEAD(&runqueue, next); 
  free(run_elt);

  running_thread->state = running;
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
    
  while(!(thread == NULL || ((Thread*)thread)->state == dead)){

    Thread * son = (Thread *)thread;
    son->father = thread_self();
    
        
    Thread * old_thread = running_thread;
    old_thread->state = blocked;
    QueueElt *run_elt = malloc(sizeof(QueueElt));
    run_elt->thread = thread_self();
    
    // Passer le premier thread de la runqueue en running
    run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
    running_thread = run_elt->thread;
    STAILQ_REMOVE_HEAD(&runqueue, next); 
    free(run_elt);
    
    running_thread->state = running;
    // Recuperer le contexte du nouveau thread courant
    // Et stocker le contexte courrant dans l'ancien thread courant
    
    swapcontext(&(old_thread->uc),&(running_thread->uc)); 
  }

  if(retval != NULL)
    *retval = ((Thread*)thread)->retval;
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

  running_thread->retval = retval;
  running_thread->state = dead;
  Thread *father = running_thread->father;
  
    //mettre le father à la fin de la runqueue
    QueueElt *run_elt = malloc(sizeof(QueueElt));
    if (father != NULL && father->state != ready){ 
      father->state = ready;
      run_elt->thread = father;
      STAILQ_INSERT_TAIL(&runqueue, run_elt, next);
    }

    //run le premier de la fifo
    if(!STAILQ_EMPTY(&runqueue)){
      run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
      STAILQ_REMOVE_HEAD(&runqueue, next);
      running_thread = run_elt->thread;
      free(run_elt);
      setcontext(&(running_thread->uc)); 
    }
}


void init(void){
  //initialisation du thread courant
  running_thread = malloc(sizeof(struct Thread));
  running_thread->id = id_ref++;//ID_FIRST_THREAD;
  running_thread->father = NULL;
  getcontext(&(running_thread->uc));

  //initialisation de la runqueue
  STAILQ_INIT(&runqueue);
}


void end(void){
  free(running_thread);
}
