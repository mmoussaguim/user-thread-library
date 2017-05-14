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
Les commandes pour utiliser le projet depuis sa racine sont:

 - Compiler le projet: `make`
 - Nettoyer le répertoire: `make clean`

###Tests

- Pour lancer tous les tests: `make test`
- Pour lancer tous les tests avec valgrind: `make test-valgrind` (attention, cela peut prendre du temps, particulièrement pour le test de fibonacci)
- Pour lancer un test un test seul: `./build/<nom du test>`
  - Les tests 21, 22, 23 et 61 requièrent un argument: le nombre de thread
  - Les tests 31 et 32 requièrent deux arguments: le nombre de thread et le nombre de yield
  - Le test 51 requière un argument: le nombre pour lequel calculer la suite de fibonnacci.

#### Fibonacci
Pour le test de Fibonacci préférer des valeurs inférieures à 30. <br>
Le test retourne rapidement jusqu'à 26, il devient beaucoup plus lent au-delà. <br>
Sans la préemption il tourne rapidement jusqu'à 30 (`GLOBAL?=-DNOPREMPTION`)

#### Préemption et priorités

Pour effectuer un test sur la préemption: compiler avec `make testpreempt` puis exécuter avec `./build/Testpreempt <nb thread>` <br>
Attention à ne pas choisir un nombre de thread trop important pour ne pas ralentir inutilement le programme (choisir un nombre de préférence entre 0 et 20)

#### Programme d'exemple

Pour compiler le programme d'exemple, exécuter la commance `make ex`. <br>
Puis pour l'éxécuter, taper `./build/Programme`.

#### Options de compilation

##### Préemption

Pour compiler sans la préemption: ajouter `GLOBAL?=-DNOPREMPTION` aux commandes ci-dessus.