#include "barrier.h"
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>

sem_t par, impar;
pthread_mutex_t mutex;

void barrier_init(barrier *b, int n) {
    b->total = n;
    b->paso = 0;
    b->bloqueadosPar = 0;
    b->bloqueadosImpar = 0;

    sem_init(&par, 0, 0);
    sem_init(&impar, 0, 0);
    pthread_mutex_init(&mutex, NULL);
}


void barrier_wait(barrier *b) {
    pthread_mutex_lock(&mutex);
    /* Soy el hilo que va parar o desbloquear los otros. */
    if (b->paso % 2 == 0) {
        /* Es un paso par.
        (estan desbloqueados los pares y se bloquean 
        como si fueran impares) */
        if (b->bloqueadosPar + 1 == b->total) {
            /* Soy el ultimo par en bloquear. Significa que ya
            tenemos que dar un paso asi que desbloqueo los impares
            y yo sigo. */
            b->paso = b->paso + 1;
            b->bloqueadosPar = 0;
            for(int i = 0; i < b->total - 1; i++)
                sem_post(&impar);
            /* Una vez todos desbloqueados suelto el mutex. */
            pthread_mutex_unlock(&mutex);
        } else {
            /* Todavia quedan pares por bloquear.Me bloqueo 
            esperando a el siguiente turno (impar) y suelto
            el lock. */
            b->bloqueadosPar = b->bloqueadosPar + 1;
            pthread_mutex_unlock(&mutex);
            sem_wait(&impar);
        }
    } else {
        /* Es un paso impar. (estan desbloqueados los impares 
        y se bloquean porque van a ser pares)*/
        if (b->bloqueadosImpar + 1 == b->total) {
            /* Soy el ulitmo impar en bloquear. Significa que ya 
            tenemos que dar un paso y desbloquera los pares. */
            b->paso = b->paso + 1;
            b->bloqueadosImpar = 0;
            for(int i = 0; i < b->total - 1; i++)
                sem_post(&par);
            /* Una vez todos desbloqueados suelto el mutex. */
            pthread_mutex_unlock(&mutex);            
        } else {
            /* Todavia quedan impares por bloquear
            Me bloqueo esperando a el siguiente turno (par) 
            y suelto el mutex. */
            b->bloqueadosImpar = b->bloqueadosImpar + 1;
            pthread_mutex_unlock(&mutex);
            sem_wait(&par);
        }
    }
}