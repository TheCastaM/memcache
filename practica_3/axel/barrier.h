#include <semaphore.h>
#include <pthread.h>

typedef struct {
    int total;
    int esperando;
    sem_t* semaforos;
    pthread_mutex_t lock;
} barrier;

void barrier_init(barrier *b, int n);

void barrier_wait(barrier *b);