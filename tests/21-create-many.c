#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "../src/thread.h"

/* test de plein de create-destroy consécutifs.
 *
 * valgrind doit etre content.
 * la durée du programme doit etre proportionnelle au nombre de threads donnés en argument.
 * jusqu'à combien de threads cela fonctionne-t-il ? -> 1010000
 *
 * support nécessaire:
 * - thread_create()
 * - thread_exit()
 * - thread_join() avec récupération de la valeur de retour
 */

static void * thfunc(void *dummy __attribute__((unused)))
{
  thread_exit(NULL);
}

int main(int argc, char *argv[])
{
  thread_t th;
  int err, i, nb;
  void *res;

  if (argc < 2) {
    printf("argument manquant: nombre de threads\n");
    return -1;
  }

  nb = atoi(argv[1]);

  for(i=0; i<nb; i++) {
    err = thread_create(&th, thfunc, NULL);
    assert(!err);
    err = thread_join(th, &res);
    assert(!err);
    assert(res == NULL);
  }

  printf("%d threads créés et détruits\n", nb);
  return 0;
}
