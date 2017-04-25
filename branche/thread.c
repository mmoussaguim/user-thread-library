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
			       //int runqueue_is_init = 0;

//File d'attente de threads en attente d'un evt
STAILQ_HEAD(ma_fifo2, QueueElt) waitqueue;
				//int waitqueue_is_init = 0;

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

extern int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg){

  
  // fonction englobante pour connnaitre la fin d'execution du thread
  void *tmp(void *arg){
    printf("--TEST-- le thread est lance\n");
    func(arg);
    printf("--TEST-- Le thread a termine\n");
    if (running_thread->state != dead)
      thread_exit(NULL);
    return NULL;
  }

  
  //Préparer le contexte
  ucontext_t uc;
  getcontext(&uc); //TODO : attraper l'erreur 
  uc.uc_stack.ss_size = 64*STACK_SIZE;
  uc.uc_stack.ss_sp = malloc(uc.uc_stack.ss_size);

    
  makecontext(&uc, (void(*)(void))*tmp, 1, funcarg);
  uc.uc_link = NULL; // ?
  
  //Créer un Thread
  Thread *new_th = malloc(sizeof(Thread));
  new_th->id = id_ref++;
  new_th->uc = uc;
  new_th->father = NULL;
  //new_th->father = running_thread;
  new_th->state = ready;

int valgrind_stackid = VALGRIND_STACK_REGISTER(uc.uc_stack.ss_sp,
                                               uc.uc_stack.ss_sp + uc.uc_stack.ss_size);
 new_th->vlg_id = valgrind_stackid;

  //Créer un QueueElt
  QueueElt *new_elt = malloc(sizeof(QueueElt));
  new_elt->thread = new_th;
  
  //L'ajouter à la runqueue
  /*if(!queue_is_init){
    STAILQ_INIT(&runqueue);
    queue_is_init = 1;
    }*/
  STAILQ_INSERT_TAIL(&runqueue, new_elt, next);
  *newthread = new_th;
  return 0;
}

extern int thread_yield(void){
  if(STAILQ_EMPTY(&runqueue))
    return 0;
  //printf("--TEST-- %p\n",runqueue.stqh_first);
  /*if (running_thread == NULL)
    return -1;*/
  
  Thread * old_thread = running_thread;
  old_thread->state = ready;
  //printf("--TEST-- running thread: %p\n",running_thread);
  // Le thread courant va dans la runqueue 
  QueueElt *run_elt = malloc(sizeof(QueueElt));
  run_elt->thread = thread_self();

  STAILQ_INSERT_TAIL(&runqueue, run_elt, next); 

  // Si l'ajout ne s'est pas bien passé, quitter
  if(STAILQ_EMPTY(&runqueue))
    return 1;
  // Passer le premier thread de la runqueue en running
  run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
  running_thread = run_elt->thread;
  //printf("--TEST-- running thread: %p\n",running_thread);
  STAILQ_REMOVE_HEAD(&runqueue, next); //ERREUR type incomplet
  free(run_elt);

  running_thread->state = running;
  // Recuperer le contexte du nouveau thread courant
  // Et stocker le contexte courrant dans l'ancien thread courant
  //printf("--TEST-- %p %p\n",&(old_thread->uc),&(running_thread->uc));
  swapcontext(&(old_thread->uc),&(running_thread->uc)); // fait un segfault (probablement quand il ne reste qu'un seul processus
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

  
  /*if (thread == NULL || ((Thread*)thread)->state == dead)
    return 0;
 
  printf("--TEST-- join debut\n");
 
  running_thread->state = blocked;
  QueueElt *run_elt = malloc(sizeof(QueueElt));
  QueueElt *new_elt = malloc(sizeof(QueueElt));

  Thread* my_thread = thread;

  //run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
    //STAILQ_REMOVE_HEAD(&runqueue, next);
  run_elt->thread = thread_self();
  STAILQ_INSERT_TAIL(&waitqueue, run_elt, next);

  printf("--TEST-- %p\n",run_elt->thread);
  printf("--TEST-- %p\n",my_thread->father);
  printf("--TEST-- %p\n",my_thread);
  //my_thread->father = run_elt->thread; // segfault sur cette ligne -> a cause de my_thread
  my_thread->father = thread_self();//running_thread;
  printf("--TEST-- join 2\n");
  new_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
  running_thread->state = blocked;
  running_thread = new_elt->thread;
  running_thread->state = running;
  // manque quelques swapcontext()
  return 0;*/





  // CC de yield:

  if(retval != NULL)
    *retval = NULL; // a gerer
  
  while(!(thread == NULL || ((Thread*)thread)->state == dead)){
    
    if( ((Thread *)thread)->father != NULL)
      printf("TEST\n");
    
    printf("--TEST-- join debut: %p\n",thread);
    Thread * son = (Thread *)thread;
    son->father = thread_self();
    //printf("--TEST-- join jusque la tout va bien\n");


    Thread * old_thread = running_thread;
    old_thread->state = blocked;
    //printf("--TEST-- running thread: %p\n",running_thread);
    // Le thread courant va dans la runqueue 
    QueueElt *run_elt = malloc(sizeof(QueueElt));
    run_elt->thread = thread_self();
    
    STAILQ_INSERT_TAIL(&waitqueue, run_elt, next); 
    
    // Si l'ajout ne s'est pas bien passé, quitter
    if(STAILQ_EMPTY(&waitqueue))
      return 1;
    // Passer le premier thread de la runqueue en running
    run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
    running_thread = run_elt->thread;
    //printf("--TEST-- running thread: %p\n",running_thread);
    STAILQ_REMOVE_HEAD(&runqueue, next); //ERREUR type incomplet
    free(run_elt);
    
    running_thread->state = running;
    // Recuperer le contexte du nouveau thread courant
    // Et stocker le contexte courrant dans l'ancien thread courant
    //printf("--TEST-- %p %p\n",&(old_thread->uc),&(running_thread->uc));

    //printf("contexte1: %p\n contecte2: %p\n",&(old_thread->uc),&(running_thread->uc));

    swapcontext(&(old_thread->uc),&(running_thread->uc)); // fait un segfault (probablement quand il ne reste qu'un seul processus
    printf("--TEST-- join2\n");    
  }
  printf("--TEST-- join fin\n");
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
  printf("--TEST-- exit debut\n");
  //écrit dans retval avant de free
  running_thread->state = dead;
  Thread *father = running_thread->father;
  
    //running_thread = father; // provisoire
    /*int k = kill(getpid(thread_self()),SIGKILL);	
      retval = &k;*/
    //mettre le father à la fin de la runqueue
    QueueElt *run_elt = malloc(sizeof(QueueElt));
    if (father != NULL && father->state != ready){ // segfault sur cette ligne
      printf("--TEST-- exit2\n");
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
      //printf("--TEST-- %d\n",running_thread->id);
      setcontext(&(running_thread->uc));
      printf("--TEST-- exit ok\n");
    }
    printf("--TEST-- exit fin\n");
  //while(1){} // Pour eviter le warning: la fonction ne doit pas retourner
}


void init(void){
  //initialisation du thread courant
  running_thread = malloc(sizeof(struct Thread));
  running_thread->id = id_ref++;//ID_FIRST_THREAD;
  running_thread->father = NULL;
  getcontext(&(running_thread->uc));

  //initialisation de la runqueue
  STAILQ_INIT(&runqueue);

  //initialisation de la waitqueue
  STAILQ_INIT(&waitqueue);

}
