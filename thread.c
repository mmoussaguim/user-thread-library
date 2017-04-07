#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include <sys/queue.h>

#define STACK_SIZE 1024

typedef struct{
  Thread *thread;
  STAILQ_ENTRY(QueueElt) next; 
}QueueElt;

/**************************************************/
/***************** LES GLOBALES *******************/
/**************************************************/

//File d'attente de threads prêts
STAILQ_HEAD(ma_fifo, QueueElt) runqueue;
int queue_is_init = 0;			    
int id_ref = 0;

//Pointeur du thread en exécution
Thread current_thread;
thread_t running_thread = &current_thread;
			    
/**************************************************/
/***************** LES FONCTIONS ******************/
/**************************************************/

/* recuperer l'identifiant du thread courant.
 */
extern thread_t thread_self(void){
  return running_thread;
}

/* creer un nouveau thread qui va exécuter la fonction func avec l'argument funcarg.
 * renvoie 0 en cas de succès, -1 en cas d'erreur.
 */
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

/* passer la main à un autre thread.
 */
extern int thread_yield(void){
  // Le thread courant passe en ready alors qu'il est sensé etre running
  // Passer le prochain thread de la FIFO ready en running
  return 0;
}

/* attendre la fin d'exécution d'un thread.
 * la valeur renvoyée par le thread est placée dans *retval.
 * si retval est NULL, la valeur de retour est ignorée.
 */
extern int thread_join(thread_t thread, void **retval){
  //le thread courrant est passé en father de 'thread', et quitte l'état running
  //run le premier de la fifo
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
  //mettre le father à la fin de la runqueue
  //run le premier de la fifo
}
