
#include <stdio.h>
#include "rwlock_bal.h"
#include <pthread.h>
#include <semaphore.h>


void write_lock(pthread_mutex_t* m , pthread_cond_t* c, pthread_cond_t* c2, int* pers, Turnos* turnos) {
    pthread_mutex_lock(m);
    turnos->numero = (turnos->numero + 1) % turnos->total;
    int minumero = turnos->numero;

    while(minumero != turnos->turno) {
        pthread_cond_wait(&c2[minumero], m);
    }
    while(*pers > 0) {
        pthread_cond_wait(c, m);
    }  
}


void write_unlock(pthread_mutex_t* m, pthread_cond_t* c2, Turnos* turnos) {
    turnos->turno = (turnos->turno + 1) % turnos->total;
    pthread_cond_signal(&c2[turnos->turno]);
    pthread_mutex_unlock(m);
}

void read_lock(pthread_mutex_t* m,pthread_cond_t* c2, int* pers, Turnos* turnos) {
    pthread_mutex_lock(m);
    turnos->numero = (turnos->numero + 1) % turnos->total;
    int minumero = turnos->numero;

    while(minumero != turnos->turno) {
        pthread_cond_wait(&c2[minumero], m);
    }
    *pers = *pers + 1;
    turnos->turno = (turnos->turno + 1) % turnos->total;
    pthread_cond_signal(&c2[turnos->turno]);
    pthread_mutex_unlock(m);
}

void read_unlock(pthread_mutex_t* m , pthread_cond_t* c, int* pers, Turnos* turnos) {
    pthread_mutex_lock(m);
    *pers = *pers - 1;
    if (*pers == 0) {
        pthread_cond_signal(c);
    }
    pthread_mutex_unlock(m);
}
