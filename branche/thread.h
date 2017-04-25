#include <ucontext.h>
#include "interface.h"

typedef enum{
  running, //en cours d'exécution
  ready,   //en attente d'être exécuté
  blocked, //non prêt en attente d'un évènement
  new,     //en cours de création
  dead,    //zombie en cours de destruction
  suspend //suspendu par l'utilisateur
} State;

typedef struct Thread{
  int id;
  ucontext_t uc;
  thread_t father;
  State state;
} Thread;



void init(void) __attribute__((constructor));
