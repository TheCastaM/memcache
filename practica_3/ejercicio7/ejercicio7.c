#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tabaco, papel, fosforos, otra_vez, esperando;

int f1,f2,f3;
int skip = 0;

void agente() {
    while (1) {
        sem_wait(&otra_vez);
        int caso = random() % 3;
        printf("Caso = %d\n", caso);
        if (caso != 0) sem_post(&fosforos);
        if (caso != 1) sem_post(&papel);
        if (caso != 2) sem_post(&tabaco);
    }
}

void fumar(int fumador) {
    printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
    f1 = 0;
    f2 = 0;
    f3 = 0;
    skip = 0;
    sleep(1);
}

void * fumador1(void *arg) {
    while (1) {
        sem_wait(&tabaco);
        //printf("Entre f1\n");
        if(skip) {
            sem_post(&tabaco);
        }
        else {
            f1 = 1;

            sem_post(&tabaco); /*Por si no me dan papel, le mando tbco*/
            sem_wait(&papel);
            if(!skip) {
                sem_wait(&tabaco); /*Por si me dieron papel, cancelo*/
                if(f3) {
   
                    skip = 1;
                    sem_post(&fosforos);
              
                    sem_wait(&esperando);
                    fumar(1);
                    sem_post(&otra_vez);

                }
                else {
                    fumar(1);
                    sem_post(&otra_vez);
                }
            }
            else sem_post(&esperando);
        }
    }
}
void * fumador2(void *arg) {
    while (1) {
        sem_wait(&fosforos);
        //printf("Entre f2\n");
        if (skip) {
            sem_post(&fosforos);
        }
        else {
            f2 = 1;
            sem_post(&fosforos); /*Por si no me dan tabaco, le mando fosforos*/
            sem_wait(&tabaco);
            if (!skip) {
                sem_wait(&fosforos); /*Por si me dieron tabaco, cancelo*/
                if (f1) {
                    skip = 1;
                    sem_post(&papel);
                    sem_wait(&esperando);
                    fumar(2);
                    sem_post(&otra_vez);

                }
                else {
                    fumar(2);
                    sem_post(&otra_vez);                    
                }
            }
            else sem_post(&esperando);
        }
    }
}
void * fumador3(void *arg) {
    while (1) {
        sem_wait(&papel);
        //printf("Entre f3\n");
        if (skip) {
            sem_post(&papel);
        }
        else {
            f3 = 1;
            
            sem_post(&papel); /*Por si no me dan fosforos, le mando papel*/
            sem_wait(&fosforos);
            if(!skip) {
                sem_wait(&papel); /*Por si me dieron fosforos, cancelo*/
                if (f2) {
                    skip = 1;
                    sem_post(&tabaco);
                    sem_wait(&esperando);
                    fumar(3);
                    sem_post(&otra_vez);
                }
                else {
                    fumar(3);
                    sem_post(&otra_vez);
                }
            }
            else sem_post(&esperando);
        }
    }
}
int main() {

    pthread_t s1, s2, s3;
    
    sem_init(&tabaco,0, 0);
    sem_init(&papel,0, 0);
    sem_init(&fosforos, 0, 0);
    sem_init(&otra_vez, 0, 1);
    
    pthread_create(&s1, NULL, fumador1, NULL);
    pthread_create(&s2, NULL, fumador2, NULL);
    pthread_create(&s3, NULL, fumador3, NULL);

    agente();
    return 0;
}

/*
a) Hay deadlock si dos fumadores distintos agarran un recurso, o
si un fumador agarra un recurso, pero el otro no era el que necesi-
taba

b)

Primer caso:
Si el agente da por ejemplo tabaco y fosforos, el fumador2 agarra
los fosforos y antes de que pueda agarrar el tabaco, lo agarra 
el fumador1, deadlock.

Podemos solucionarlo intercambiando ? 

Supongamos sin perder gralidad:

fum 1 :
tabaco
papel


fum 2 :
tabaco
fosforos

fum 3:
papel
fosforos

Si reordenamos , igual puede haber deadlock si fum1 agarra primero.

En conclusion: No, sin importar como reorganizemos los recursos
siempre podemos caer en Deadlock.
*/