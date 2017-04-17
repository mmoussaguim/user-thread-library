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
