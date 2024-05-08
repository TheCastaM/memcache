#include "barrier.h"
#include <stdio.h>

#define Green "\033[0;32m"
#define Red "\033[0;31m"
#define End "\033[0m"



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

    // b->esperando = b->esperando + 1;
    // printf("Entre... %d\n", b->esperando);
    
    if(b->esperando + 1 == b->total) {
        b->esperando = 0;
        
        printf(Green"Yo desbloqueo\n"End);
        
        for(int i = 0; i < b->total - 1; i++) 
            sem_post(&b->semaforos[i]);
        

        pthread_mutex_unlock(&b->lock);
        printf("Soy el numero: %d\n", b->total-1);

    } else {
        int i = b->esperando;
        b->esperando = b->esperando + 1;
        
        // printf("Bloqueando %d\n", i);
        
        pthread_mutex_unlock(&b->lock);
        // printf(Red"Dejo el mutex %d\n"End, i);

        printf("Soy el numero: %d\n", i);

        sem_wait(&b->semaforos[i]);
        // printf("Me desbloquearon %d\n", i);
    }
}