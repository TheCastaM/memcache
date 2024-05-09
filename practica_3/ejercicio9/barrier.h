#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct {
    int total; /* Cant. total de hilos. */
    int paso;
    int bloqueadosPar; /* Cant. de hilos bloqueados. */
    int bloqueadosImpar; /* Cant. de hilos bloqueados. */
} barrier;

void barrier_init(barrier *b, int n);

void barrier_wait(barrier *b);