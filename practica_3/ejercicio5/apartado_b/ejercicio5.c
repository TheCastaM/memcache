#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "rwlock.h"

#define Green "\033[0;32m"
#define Red "\033[0;31m"
#define End "\033[0m"

#define M 5
#define N 5
#define ARRLEN 10240
int arr[ARRLEN];


int pers = 0; /* Cant. personas en el archivo. */
pthread_mutex_t mutex;
pthread_cond_t cond;

void * escritor(void *arg) 
{
    int i;
    int num = arg - (void*)0; /* Num. de escritor. */
    while (1) {
        sleep(random() % 3);
        
        write_lock(&mutex, &cond, &pers);
        
        printf(Red "Escritor " End "%d escribiendo\n", num);

        for (i = 0; i < ARRLEN; i++)
            arr[i] = num;
        
        write_unlock(&mutex, &pers);
    }
    return NULL;
}

void * lector(void *arg) 
{
    int v, i;
    int num = arg - (void*)0; /* Num. de lector. */
    while (1) {
        sleep(random() % 3);

        read_lock(&mutex, &cond, &pers);
        
        v = arr[0];
        for (i = 1; i < ARRLEN; i++) {
            if (arr[i] != v)
                break;
        }

        if (i < ARRLEN)
            printf(Green"Lector "End"%d, error de lectura\n", num);
        else
            printf(Green"Lector "End"%d, dato %d\n", num, v); 
        
        read_unlock(&mutex, &cond, &pers);
    }
    return NULL;
}

int main() 
{
    pthread_t lectores[M], escritores[N];
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    
    int i;
    for (i = 0; i < N; i++)
        pthread_create(&escritores[i], NULL, escritor, i + (void*)0);

    for (i = 0; i < M; i++)
        pthread_create(&lectores[i], NULL, lector, i + (void*)0);
    
    pthread_join(lectores[0], NULL); /* Espera para siempre */
    return 0;
}