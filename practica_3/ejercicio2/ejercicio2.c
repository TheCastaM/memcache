#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;
sem_t semaphore2;

void *funcion_a(void *arg)
{
    puts("a1");
    sem_wait(&semaphore2);
    sem_post(&semaphore);
    puts("a2");
}

void *funcion_b(void *arg)
{
    puts("b1");
    sem_post(&semaphore2);
    sem_wait(&semaphore);
    puts("b2");
}

int main() 
{
    sem_init(&semaphore, 0, 0);
    sem_init(&semaphore2, 0, 0);

    puts("New:");
    pthread_t ta, tb;

    pthread_create(&ta, NULL, funcion_a, NULL);
    pthread_create(&tb, NULL, funcion_b, NULL);

    pthread_join(ta, NULL);
    pthread_join(ta, NULL);
    printf("\n\n\n");

}