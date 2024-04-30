#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int waitA = 1;
int waitB = 1;

void *funcion_a(void *arg)
{

    puts("a1");
    waitA = 0;
    while (waitB)
        ;
    puts("a2");
}

void *funcion_b(void *arg)
{
    while (waitA)
        ;
    puts("b1");
    waitB = 0;
    puts("b2");

}

int main() 
{
    puts("New:");
    pthread_t ta, tb;

    pthread_create(&ta, NULL, funcion_a, NULL);
    pthread_create(&tb, NULL, funcion_b, NULL);

    pthread_join(ta, NULL);
    pthread_join(ta, NULL);
    printf("\n\n\n");

}