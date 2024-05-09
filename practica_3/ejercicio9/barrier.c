#include "barrier.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define Green "\033[0;32m"
#define Red "\033[0;31m"
#define End "\033[0m"



void barrier_init(barrier *b, int n) {
    b->esperando = 0;
    b->total = n;

    for(int i = 0; i < b->total; i++)
        b->arrSemaforos[i] = 0;
    
    for(int i = 0; i < n; i++) {
        sem_init(&b->semaforos[i], 0, 0);   
    }
    pthread_mutex_init(&b->lock, NULL);
}

int semaforo_idx(pid_t pid, pid_t* arr) {
    for (int i = 0; i < sizeof(arr); i++) {
        if (arr[i] == pid) {
            return i;
        } else if (arr[i] == 0) {
            arr[i] = pid;
            return i;
        }
    }
}

void barrier_wait(barrier *b) {
    pthread_mutex_lock(&b->lock);

    // b->esperando = b->esperando + 1;
    // printf("Entre... %d\n", b->esperando);
    
    if(b->esperando + 1 == b->total) {
        b->esperando = 0;
        
        printf(Green"Yo desbloqueo\n"End);
        
        int idx = semaforo_idx(getpid(), b->arrSemaforos);

        for(int i = 0; i < b->total - 1; i++) {
            if (i != idx)
                sem_post(&b->semaforos[i]);
        }

        pthread_mutex_unlock(&b->lock);
        printf("Soy el numero: %d\n", b->total-1);

    } else {
        b->esperando = b->esperando + 1;
        // printf("Bloqueando %d\n", i);

        int i = semaforo_idx(getpid(), b->arrSemaforos);

        pthread_mutex_unlock(&b->lock);
        // printf(Red"Dejo el mutex %d\n"End, i);
        // printf("Soy el numero: %d\n", i);

        sem_wait(&b->semaforos[i]);
        // printf("Me desbloquearon %d\n", i);
    }
}