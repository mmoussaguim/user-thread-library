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
