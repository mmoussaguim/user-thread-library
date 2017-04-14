void init(){
  //initialisation du thread courant
  running_thread = malloc(sizeof(struct Thread));
  running_thread->id = ID_FIRST_THREAD;
  running_thread->father = NULL;
  get_context(&(running_thread->uc));

  //initialisation de la runqueue
  STAILQ_INIT(&runqueue);

  //initialisation de la waitqueue
  //STAILQ_INIT(&waitqueue);

}
