#include "cola.h"
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    Cola cola;
    pthread_mutex_t *mutex;
} cond;

void cond_init (cond* condicional);

void cond_wait (cond* condicional, pthread_mutex_t* mutex);

void cond_signal (cond* condicional);

void cond_broadcast (cond* condicional);