#include <semaphore.h>
#include <stdlib.h>

typedef struct _Nodo{
    sem_t* semaforo;
    struct _Nodo* sig;
} *Cola;

typedef int (*FuncionDestructora)(sem_t* sem);

Cola cola_crea() ;

Cola cola_insert(Cola cola, void* dato) ;

void* cola_top(Cola cola) ;

Cola cola_pop(Cola cola, FuncionDestructora funcion) ;