Bibliothèque de threads en espace utilisateur
==============================================

Ce projet vise à construire une bibliothèque de threads en espace utilisateur. On va donc fournir une interface de programmation plus ou moins proche des pthreads, mais en les exécutant sur un seul thread noyau. Les intérêts sont:

   * Les coûts d'ordonnancement sont beaucoup plus faibles
   * Les politiques d'ordonnancement sont configurables
   * On peut enfin expérimenter le changement de contexte pour de vrai

Objectifs avancés
------------------------

Les objectifs avancés ayant été implémentés sont:

 - La préemption
 - La gestion priorités:
	 - De 0 à 20, du plus au moins prioritaire
 - L'implémentation des mutex

Instructions 
-----------------
Les commandes pour utiliser les projet depuis sa racine sont:

 - Compiler le projet: `make`
 - Nettoyer le répertoire: `make clean`

###Tests

- Pour lancer tous les tests: `make test`
- Pour lancer tous les tests avec valgrind: `make test-valgrind` (attention, cela peut prendre du temps, particulièrement pour le test de fibonacci)
- Pour lancer un test un test seul: `./build/<nom du test>`

#### Fibonacci
Pour le test de Fibonacci préférer des valeurs inférieurs à 35.

#### Préemption

Pour effectuer un test sur la préemption: compiler avec `make testpreempt` puis exécuter avec `./build/Testpreempt` <br>
Ce test est constitué d'une boucle infini, il ne termine donc pas. <br>
Attention à bien s'assurer qu'un `printf`est présent dans la fonction `preempt` de `thread.c`, sinon le programme semblera tourner dans le vide.

#### Programme d'exemple

Pour compiler le programme d'exemple, éxécuter la commance `make ex`. <br>
Puis pour l'éxécuter, taper `./Programme`.

#### Options de compilation

##### Préemption

Pour compiler sans la préemption: ajouter `GLOBAL?=-DNOPREMPTION` aux commandes ci-dessus.