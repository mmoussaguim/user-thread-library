
/* attendre la fin d'exécution d'un thread.
 * la valeur renvoyée par le thread est placée dans *retval.
 * si retval est NULL, la valeur de retour est ignorée.
 */
extern int thread_join(thread_t thread, void **retval){
  //le thread courant est passé en father de 'thread', et quitte l'état running
  //run le premier de la fifo
  // Attention gérer le cas ou le htread a deja terminé


  QueueElt *run_elt = malloc(sizeof(QueueElt));
  QueueElt *new_elt = malloc(sizeof(QueueElt));

  Thread* my_thread = thread;

  run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
  STAILQ_REMOVE_HEAD(&runqueue, next); 
  STAILQ_INSERT_TAIL(&waitqueue, run_elt, next);

  my_thread->father = run_elt->thread;

  new_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
  running_thread = new_elt->thread;




  return 0;
}
