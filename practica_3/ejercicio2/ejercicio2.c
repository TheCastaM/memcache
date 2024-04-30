#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int wait = 1;

void *funcion_a(void *arg)
{

    puts("a1");
    puts("a2");
    wait = 0;
}

void *funcion_b(void *arg)
{
    while (wait)
        ;

    puts("b1");
    puts("b2");

}

int main() 
{
    pthread_t ta, tb;

    pthread_create(&ta, NULL, funcion_a, NULL);
    pthread_create(&tb, NULL, funcion_b, NULL);

    pthread_join(ta, NULL);
    pthread_join(ta, NULL);

}