#include <stdio.h>
#include "rwlock.h"
#include <pthread.h>

void write_lock(pthread_mutex_t* m, pthread_cond_t* c, int* pers) {
    pthread_mutex_lock(m);
    while(*pers>0) {
        //printf("Hay personas esperando...\n");
        pthread_cond_wait(c,  m);
    }
    //printf("Salgo de la espera...\n");
}

void write_unlock(pthread_mutex_t* m, int* pers) {
    
    pthread_mutex_unlock(m);
}


void read_lock(pthread_mutex_t* m, pthread_cond_t* c, int* pers) {
    pthread_mutex_lock(m);
    *pers = *pers + 1;
    //printf("Lectores = %d\n", *pers);
    pthread_mutex_unlock(m);

}

void read_unlock(pthread_mutex_t* m, pthread_cond_t* c, int* pers) {
    pthread_mutex_lock(m);
    *pers = *pers - 1;
    if (*pers == 0) {
        pthread_cond_signal(c);
    }
    pthread_mutex_unlock(m);
}