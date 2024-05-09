#include "cond.h"
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>


void cond_init (cond* condicional) {
    pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mutex, NULL);
    condicional->mutex = mutex;
    
    Cola newCola = cola_crea();
    condicional->cola = newCola;
}

void cond_wait (cond* condicional, pthread_mutex_t* mutex) {
    pthread_mutex_lock(condicional->mutex);
    
    sem_t* newSemaforo = malloc(sizeof(sem_t));
    sem_init(newSemaforo, 0, 0);
    condicional->cola = cola_insert(condicional->cola, newSemaforo);
    
    pthread_mutex_unlock(condicional->mutex);
    pthread_mutex_unlock(mutex);
    
    sem_wait(newSemaforo);

    pthread_mutex_lock(mutex);
}

void cond_signal (cond* condicional) {
   
    pthread_mutex_lock(condicional->mutex);

    if (cola_top(condicional->cola) != NULL) {
        sem_t* semaforo = cola_top(condicional->cola);
        sem_post(semaforo);
    }

    condicional->cola = cola_pop(condicional->cola, sem_destroy);

    pthread_mutex_unlock(condicional->mutex);
}

void cond_broadcast (cond* condicional) {
    pthread_mutex_lock(condicional->mutex);

    sem_t* tempSemaforo = cola_top(condicional->cola);
    while (tempSemaforo != NULL) {
        sem_post(tempSemaforo);
        sem_destroy(tempSemaforo);
        pthread_mutex_lock(condicional->mutex);
        condicional->cola = cola_pop(condicional->cola, sem_destroy);
        pthread_mutex_unlock(condicional->mutex);
        tempSemaforo = cola_top(condicional->cola);
    }

    pthread_mutex_unlock(condicional->mutex);
}