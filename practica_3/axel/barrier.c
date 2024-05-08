#include "barrier.h"
#include <stdio.h>
void barrier_init(barrier *b, int n) {
    b->esperando = 0;
    b->total = n;
    for(int i = 0; i < n; i++) {
        sem_init(&b->semaforos[i], 0, 0);
    }
    pthread_mutex_init(&b->lock, NULL);
}

void barrier_wait(barrier *b) {
    pthread_mutex_lock(&b->lock);
    //b->esperando = b->esperando + 1;
    printf("Entre... %d\n", b->esperando);
    if(b->esperando + 1 == b->total) {
        b->esperando = 0;
        for(int i = 0; i < b->total - 1; i++) {
            //printf("Desbloqueando %d\n", i -1 );
            sem_post(&b->semaforos[i]);
        }
        pthread_mutex_unlock(&b->lock);
    }
    else {
        int i = b->esperando;
        b->esperando = b->esperando + 1;
        printf("Bloqueando %d\n", i);
        pthread_mutex_unlock(&b->lock);
                printf("Sali... %d\n", b->esperando);

        sem_wait(&b->semaforos[i]);
    }
}