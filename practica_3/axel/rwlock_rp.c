#include <stdio.h>
#include "rwlock_rp.h"
#include <pthread.h>

void write_lock(pthread_mutex_t* m, pthread_cond_t* c, int* pers, int* w) {
    pthread_mutex_lock(m);
    *w = *w + 1;
    printf("Escritor va a entrar...\n");
    while(*pers>0) {
        pthread_cond_wait(c,  m);
    }
}

void write_unlock(pthread_mutex_t* m, pthread_cond_t* c, int* w) {
    *w = *w - 1;
    printf("Writers = %d\n", *w);
    if (*w == 0) {
        pthread_cond_signal(c);
    }
    pthread_mutex_unlock(m);
}


void read_lock(pthread_mutex_t* m, pthread_cond_t* c, int* pers, int* w) {
    pthread_mutex_lock(m);
    //printf("Writers esperando : %d\n", *w);
    while(*w>0) {
        printf("Escritor quiere entrar, espero...\n");
        pthread_cond_wait(c,m);
    }
    *pers = *pers + 1;
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