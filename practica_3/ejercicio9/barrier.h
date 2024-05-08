#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct {
    int total;  /* Total de hilos. */
    int esperando; /* Cant. de hilos esperando. */
    sem_t* semaforos; /* Semaforos de los hilos esperando. */
    pthread_mutex_t lock; /* Modifica el esperando. */
} barrier;

void barrier_init(barrier *b, int n);

void barrier_wait(barrier *b);