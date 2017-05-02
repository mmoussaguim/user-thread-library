#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include <sys/queue.h>
#include <ucontext.h> /* ne compile pas avec -std=c89 ou -std=c99 */
#include <unistd.h>


#define STACK_SIZE 1024
#define PREEMPT_TIME 50000// en usec

typedef struct QueueElt{
  Thread *thread;
  STAILQ_ENTRY(QueueElt) next; 
}QueueElt;

/**************************************************/
/***************** LES GLOBALES *******************/
/**************************************************/
//File d'attente de threads prêts
STAILQ_HEAD(ma_fifo, QueueElt) runqueue;

//FIFO contenant un pointeur vers tous les threads (pour libération)
STAILQ_HEAD(ma_fifo2, QueueElt) deletequeue;


//Pointeur du thread en exécution
Thread current_thread; // à initialiser ?
Thread* running_thread = &current_thread;


/**************************************************/
/***************** LES FONCTIONS ******************/
/**************************************************/



/*
  Si dans exit passer NULL en argument
  Retourne 0 si un thread de la runqueue a ete lance
  1 si la runqueue est vide
 */
int run_other_thread(Thread * old_thread){

  if(STAILQ_EMPTY(&runqueue))
    return 1;
  
  QueueElt *run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
  running_thread = run_elt->thread;
  STAILQ_REMOVE_HEAD(&runqueue, next); 
  free(run_elt);

  /********** PREEMPTION *****************/
  alarm(1); // ualarm()
  signal(SIGALRM,preempt);
  /********** PREEMPTION *****************/
  //running_thread->state = running;
  
  if(old_thread != NULL)
    swapcontext(&(old_thread->uc),&(running_thread->uc));
  else 
    setcontext(&(running_thread->uc));

  /********** PREEMPTION *****************/
  //alarm(1); 
  ualarm(PREEMPT_TIME,0);
  signal(SIGALRM,preempt);
  /********** PREEMPTION *****************/

  return 0;
}


/*
  Signal handler
  Fonction permettant de gérer la préemption
  A passer en argument de signal dans run_other_thread
 */
void preempt(int signum){ 
  //running_thread->state = ready;
  printf("--TEST-- preemption du thread %p\n",running_thread);
  QueueElt *run_elt = malloc(sizeof(QueueElt));
  run_elt->thread = thread_self();
  STAILQ_INSERT_TAIL(&runqueue, run_elt, next);

  run_other_thread(running_thread);
}


int free_thread(Thread ** thread){
  printf("--TEST-- freethread\n");
  if(thread != NULL && *thread != NULL){
    
    if((*thread)->uc.uc_stack.ss_sp != NULL){
      printf("--TEST-- freethread %p\n",(*thread)->uc.uc_stack.ss_sp);
      free((*thread)->uc.uc_stack.ss_sp);
      (*thread)->uc.uc_stack.ss_sp = NULL;
      }
    //printf("--TEST-- freethread thread:%p\n",*thread);
    
    free(*thread);
    *thread = NULL;
    return 0;
  }
  return 1;
}

extern thread_t thread_self(void){
  //printf("--TEST-- self\n");
  return running_thread;
}


// fonction englobante pour connnaitre la fin d'execution du thread
void *tmp(void* (*func)(void*), void *arg){
  void * retval = func(arg);
  if (!running_thread->is_dead)
    thread_exit(retval);
  return retval;
  }



extern int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg){   
  //Préparer le contexte
  static ucontext_t uc;
  getcontext(&uc); //TODO : attraper l'erreur 
  uc.uc_stack.ss_size = 64*STACK_SIZE;
  uc.uc_stack.ss_sp = malloc(uc.uc_stack.ss_size);
  //static char stack[64*STACK_SIZE];
  //uc.uc_stack.ss_sp = stack;

  makecontext(&uc, (void(*)(void))*tmp, 2,func, funcarg);
  uc.uc_link = NULL;
  
  //Créer un Thread
  Thread *new_th = malloc(sizeof(Thread));
  new_th->uc = uc;
  new_th->father = NULL;
  new_th->retval = NULL;
  new_th->is_dead = 0; 
  //Référencer ce thread sur newthread
  *newthread = new_th;
  
  //Valgrind
  int valgrind_stackid = VALGRIND_STACK_REGISTER(uc.uc_stack.ss_sp,
						 uc.uc_stack.ss_sp + uc.uc_stack.ss_size);
  new_th->vlg_id = valgrind_stackid;

  //Créer un QueueElt pour chaque file
  QueueElt *new_elt = malloc(sizeof(QueueElt));
  new_elt->thread = new_th;
  QueueElt *new_elt2 = malloc(sizeof(QueueElt));
  new_elt2->thread = new_th;
  //L'ajouter à la runqueue et à la deletequeue
  STAILQ_INSERT_TAIL(&runqueue, new_elt, next);
  STAILQ_INSERT_TAIL(&deletequeue, new_elt2, next);

  return 0;
}





extern int thread_yield(void){
  if(STAILQ_EMPTY(&runqueue))
    return 0;
    
  //Thread * old_thread = running_thread;
  QueueElt *run_elt = malloc(sizeof(QueueElt));
  run_elt->thread = thread_self();

  STAILQ_INSERT_TAIL(&runqueue, run_elt, next); 

  // Si l'ajout ne s'est pas bien passé, quitter
  /*if(STAILQ_EMPTY(&runqueue))
    return 1;
  // Passer le premier thread de la runqueue en running
  run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
  running_thread = run_elt->thread;
  STAILQ_REMOVE_HEAD(&runqueue, next); 
  free(run_elt);

  // Recuperer le contexte du nouveau thread courant
  // Et stocker le contexte courrant dans l'ancien thread courant
  swapcontext(&(old_thread->uc),&(running_thread->uc)); */
  run_other_thread(running_thread);
  return 0;
}





/* attendre la fin d'exécution d'un thread.
 * la valeur renvoyée par le thread est placée dans *retval.
 * si retval est NULL, la valeur de retour est ignorée.
 */
extern int thread_join(thread_t thread, void **retval){  
  if(!(thread == NULL || ((Thread*)thread)->is_dead)){
    // Renseigner le père au thread que l'on va attendre (son)
    Thread * son = (Thread *)thread;
    son->father = thread_self();
      
    // Passer l'état du thread courrant en blocked
    /*Thread * old_thread = running_thread;
    
    // Passer le premier thread de la runqueue en running
    QueueElt * run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
    running_thread = run_elt->thread;
    STAILQ_REMOVE_HEAD(&runqueue, next); 
    free(run_elt);

    // Recuperer le contexte du nouveau thread courant
    // Et stocker le contexte courrant dans l'ancien thread courant    
    swapcontext(&(old_thread->uc),&(running_thread->uc)); */
    run_other_thread(running_thread);
  }

  if(retval != NULL)
    *retval = ((Thread*)thread)->retval;
  return 0;  
}

extern void thread_exit(void *retval){
  //écrit dans retval avant de free
  running_thread->retval = retval;
  running_thread->is_dead = 1;
  Thread *father = running_thread->father;
  
  //mettre le father à la fin de la runqueue
  QueueElt *run_elt;
  if (father != NULL){ 
    run_elt = malloc(sizeof(QueueElt));
    run_elt->thread = father;
    STAILQ_INSERT_TAIL(&runqueue, run_elt, next);
  }

  //run le premier de la fifo
  /*if(!STAILQ_EMPTY(&runqueue)){
    run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
    running_thread = run_elt->thread;
    STAILQ_REMOVE_HEAD(&runqueue, next);
    free(run_elt);
    setcontext(&(running_thread->uc)); 
    }*/
  run_other_thread(NULL);
  exit(0);//while(1); 
}


void init(void){
  //initialisation du thread courant
  running_thread = malloc(sizeof(struct Thread));
  running_thread->father = NULL;
  getcontext(&(running_thread->uc));
  running_thread->uc.uc_stack.ss_size = 64*STACK_SIZE;
  // static char stack[64*STACK_SIZE];
  //running_thread->uc.uc_stack.ss_sp = stack;
  running_thread->uc.uc_stack.ss_sp = malloc(running_thread->uc.uc_stack.ss_size);

  //initialisation de la runqueue et deletequeue
  STAILQ_INIT(&runqueue);
  STAILQ_INIT(&deletequeue);

  //ajout du thread main dans la deletequeue
  QueueElt *run_elt = malloc(sizeof(QueueElt));
  run_elt->thread = thread_self();
  STAILQ_INSERT_TAIL(&deletequeue, run_elt, next); 
}


void end(void){
  //free(running_thread); //nope normalement il est dans la deletequeue
  QueueElt *elt;
  QueueElt *previous_elt = NULL;
  STAILQ_FOREACH(elt, &deletequeue, next){
    printf("--TEST-- end elt:%p->%p\n",elt,elt+sizeof(QueueElt));
    if(previous_elt != NULL)
      free(previous_elt);
    free_thread(&(elt->thread));
    previous_elt = elt;
  }
  if(previous_elt != NULL)
    free(previous_elt);
}
