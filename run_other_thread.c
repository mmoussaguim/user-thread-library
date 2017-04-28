
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

  //running_thread->state = running;
  
  if(old_thread != NULL)
    swapcontext(&(old_thread->uc),&(running_thread->uc));
  else 
    setcontext(&(running_thread->uc));
  return 0;
}
