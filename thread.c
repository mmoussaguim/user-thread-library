#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#include "interface.h"
#include <sys/queue.h>

#define STACK_SIZE 1024

typedef enum{
  running, //en cours d'exécution
  ready,   //en attente d'être exécuté
  blocked, //non prêt en attente d'un évènement
  new,     //en cours de création
  exit,    //zombie en cours de destruction
  suspend //suspendu par l'utilisateur
} State;

struct QueueElt{
  struct Thread thread;
  STAILQ_ENTRY(struct QueueElt);
};

struct Thread{
  int id;
  ucontext_t uc;
  State state; //remove ?
  thread_t father;
};

// RUNQUEUE File d'attente de threads prêts - Simple Queue
STAILQ_HEAD(fifo, QueueElt) runqueue;

// Pointeur du thread en exécution
thread_t running_thread;

/* recuperer l'identifiant du thread courant.
 */
extern thread_t thread_self(void){
  //retourne identifiant du thread courrant
  if (running_thread != NULL)
    return running_thread->id;
  else
    return NULL;
}

/* creer un nouveau thread qui va exécuter la fonction func avec l'argument funcarg.
 * renvoie 0 en cas de succès, -1 en cas d'erreur.
 */
extern int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg){
  // Créer (malloc) thread et ajouter à la liste globale en mode ready
}

/* passer la main à un autre thread.
 */
extern int thread_yield(void){
  // Le thread courant passe en ready alors qu'il est sensé etre running
  // Passer le prochain thread de la FIFO ready en running
}

/* attendre la fin d'exécution d'un thread.
 * la valeur renvoyée par le thread est placée dans *retval.
 * si retval est NULL, la valeur de retour est ignorée.
 */
extern int thread_join(thread_t thread, void **retval){
  //le thread courrant est passé en father de 'thread', et quitte l'état running
  //run le premier de la fifo
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
