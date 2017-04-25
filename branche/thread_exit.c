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
	
	Thread *father = current_thread.father;
	
	int k = kill(getpid(thread_self()),SIGKILL);	
	retval = &k;
	//mettre le father à la fin de la runqueue
	QueueElt *run_elt = malloc(sizeof(QueueElt));
  	run_elt->thread = father;
	STAILQ_INSERT_TAIL(&runqueue, run_elt, next);

  	//run le premier de la fifo
	
	run_elt = (QueueElt *) STAILQ_FIRST(&runqueue);
	STAILQ_REMOVE_HEAD(&runqueue, next); //ERREUR type incomplet
  	running_thread = run_elt->thread;
  	free(run_elt);
}
