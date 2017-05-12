#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include <sys/queue.h>
#include <ucontext.h> /* ne compile pas avec -std=c89 ou -std=c99 */
#include <unistd.h>

#ifndef DEBUG
#define DEBUG 1
#endif

#define debug_printf(...) \
            do { if (DEBUG) fprintf(stderr, __VA_ARGS__); } while (0)


#define STACK_SIZE 1024
#define PREEMPT_TIME 8000 // en usec
#define DEFAULT_PRIORITY 10
#define MAX_PRIO 20
#define MIN_PRIO 0

typedef struct QueueElt{
  Thread *thread;
  STAILQ_ENTRY(QueueElt) next; 
}QueueElt;

/**************************************************/
/***************** LES GLOBALES *******************/
/**************************************************/
//File d'attente de threads prêts
STAILQ_HEAD(ma_fifo, Thread) runqueue;

//Pointeur du thread en exécution
Thread current_thread; // à initialiser ?
Thread* running_thread = &current_thread;
Thread* thmain;
Thread* lasttofree;

/**************************************************/
/***************** LES FONCTIONS ******************/
/**************************************************/



/*
  Si dans exit passer NULL en argument
  Retourne 0 si un thread de la runqueue a ete lance
  1 si la runqueue est vide
 */
int run_other_thread(Thread * old_thread){
  mask_signal(SIGALRM);
  if(STAILQ_EMPTY(&runqueue))
    return 1;
  
  Thread *run_elt = (Thread *) STAILQ_FIRST(&runqueue);
  running_thread = run_elt;
  STAILQ_REMOVE_HEAD(&runqueue, next); 
  unmask_signal(SIGALRM);

  /********** PREEMPTION *****************/
#ifndef NOPREMPTION
  ualarm(preemptime(running_thread),0);
#endif
  /********** PREEMPTION *****************/
  
  if(old_thread != NULL)
    swapcontext(old_thread->uc,running_thread->uc);
  else 
    setcontext(running_thread->uc);

  return 0;
}

int preemptime(Thread * thread){
  if(thread == NULL)
    return -1;
  return PREEMPT_TIME + ((PREEMPT_TIME * (10 - thread->priority))/10) / 2;
}

/*
  Signal handler
  Fonction permettant de gérer la préemption
  A passer en argument de signal dans run_other_thread
 */
void preempt(int signum){ 
  insert_runqueue(running_thread);
  run_other_thread(running_thread);
}

void insert_runqueue(Thread *thread){
  //Masquer les signaux SIGALRM
  mask_signal(SIGALRM);
  //Ajouter à la runqueue
  STAILQ_INSERT_TAIL(&runqueue, thread, next);
  //Démasquer les signaux SIGALRM
  unmask_signal(SIGALRM);
}

int free_thread(Thread ** thread){
  if((*thread) == thmain)
    return 0;//thmain = NULL;

  if(thread != NULL && *thread != NULL){
    
    if((*thread)->uc->uc_stack.ss_sp != NULL){
      free((*thread)->uc->uc_stack.ss_sp);
      (*thread)->uc->uc_stack.ss_sp = NULL;


      free((*thread)->uc);
    }
    
    free(*thread);
    *thread = NULL;
    return 0;
  }   
  return 1;
}

/* Récupère le masque courrant, ajoute signum, et bloque */
void mask_signal(int signum){
  sigset_t set;
  sigprocmask(SIG_SETMASK,&set,NULL);
  sigaddset(&set, signum);
  int rc = sigprocmask(SIG_BLOCK, &set, NULL); 
  if(rc != 0)
    perror("sigprocmask BLOCK mal passé\n");
}

/* Débloque tous les signaux du masque courrant */
void unmask_signal(int signum){
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set,signum);
  int rc = sigprocmask(SIG_UNBLOCK, &set, NULL); 
  if(rc != 0)
    perror("sigprocmask UNBLOCK mal passé\n");
}

extern thread_t thread_self(void){
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
  ucontext_t* uc = malloc(sizeof(ucontext_t));
  getcontext(uc); //TODO : attraper l'erreur 
  uc->uc_stack.ss_size = 64*STACK_SIZE;
  uc->uc_stack.ss_sp = malloc(uc->uc_stack.ss_size);

  makecontext(uc, (void(*)(void))*tmp, 2,func, funcarg);
  uc->uc_link = NULL;
  
  //Créer un Thread
  Thread *new_th = malloc(sizeof(Thread));
  new_th->uc = uc;
  new_th->father = NULL;
  new_th->retval = NULL;
  new_th->is_dead = 0; 
  new_th->priority = DEFAULT_PRIORITY;

  //Référencer ce thread sur newthread
  *newthread = new_th;
  
  //Valgrind
  int valgrind_stackid = VALGRIND_STACK_REGISTER(uc->uc_stack.ss_sp,
						 uc->uc_stack.ss_sp + uc->uc_stack.ss_size);
  new_th->vlg_id = valgrind_stackid;

  //Ajouter le nouveau thread à la runqueue
  insert_runqueue(new_th);

  return 0;
}

int thread_setschedprio(Thread * thread, int prio){
  if (thread == NULL)
    return -1;
  if (prio > MAX_PRIO || prio < MIN_PRIO)
    return -1;

  thread->priority = prio;
  return 0;
}



extern int thread_yield(void){
  if(STAILQ_EMPTY(&runqueue))
    return 0;  
  insert_runqueue(running_thread);
  run_other_thread(running_thread);
  return 0;
}

/* attendre la fin d'exécution d'un thread.
 * la valeur renvoyée par le thread est placée dans *retval.
 * si retval est NULL, la valeur de retour est ignorée.
 */
extern int thread_join(thread_t thread, void **retval){  
    Thread * son = (Thread *)thread;
  if(!(thread == NULL || son->is_dead)){
    // Renseigner le père au thread que l'on va attendre (son)
    son->father = thread_self();
    // Changer de thread courrant
    run_other_thread(running_thread);
  }
  if(retval != NULL)
    *retval = ((Thread*)thread)->retval;
  free_thread(&son);
  return 0;  
}

extern void thread_exit(void *retval){
  //écrit dans retval avant de free
  running_thread->retval = retval;
  running_thread->is_dead = 1;
  Thread *father = running_thread->father;
  
  //mettre le father à la fin de la runqueue
  if (father != NULL){ 
    insert_runqueue(father);
  }

  //run le premier de la fifo
  if(!STAILQ_EMPTY(&runqueue))
    run_other_thread(NULL);
  if(running_thread != thmain){
    lasttofree = running_thread;
    setcontext(thmain->uc);
  }
  exit(0);
}


void init(void){
  //initialisation du thread courant
  running_thread = malloc(sizeof(struct Thread));
  thmain = running_thread;
  lasttofree = NULL;
  running_thread->father = NULL;
  running_thread->uc = malloc(sizeof(ucontext_t));
  getcontext(running_thread->uc);
  running_thread->uc->uc_stack.ss_size = 64*STACK_SIZE;
  running_thread->uc->uc_stack.ss_sp = malloc(running_thread->uc->uc_stack.ss_size);
  running_thread->priority = DEFAULT_PRIORITY;
  //initialisation de la runqueue et deletequeue
  STAILQ_INIT(&runqueue);
  //Ajout de l'alarme de préemption pour le premier thread 
#ifndef NOPREMPTION
  signal(SIGALRM,preempt); 
  ualarm(preemptime(running_thread),0);
#endif
}


void end(void){
  debug_printf("--TEST-- Destructeur\n");
  
  if(thmain != NULL){
    free(thmain->uc->uc_stack.ss_sp);
    free(thmain->uc);
    free(thmain);
    thmain = NULL;
  }
  if(lasttofree != NULL){
    free(lasttofree->uc);
    free(lasttofree);
    thmain = NULL;
    }
}
