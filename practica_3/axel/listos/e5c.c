#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "rwlock_bal.h"


#define M 5
#define N 5
#define ARRLEN 10240
int arr[ARRLEN];


int pers = 0;
pthread_mutex_t mutex;
pthread_cond_t no_personas_sala;
pthread_cond_t cond_turnos[M+N];

Turnos turn;

void * escritor(void *arg) {
    int i;
    int num = arg - (void*)0;
    while (1) {
        sleep(random() % 3);
        write_lock(&mutex, &no_personas_sala, cond_turnos, &pers, &turn);
        printf("Escritor %d escribiendo\n", num);
        for (i = 0; i < ARRLEN; i++)
        arr[i] = num;
        write_unlock(&mutex, cond_turnos, &turn);
    }
    return NULL;
}
void * lector(void *arg) {
    int v, i;
    int num = arg - (void*)0;
    while (1) {
        sleep(random() % 3);
        read_lock(&mutex, cond_turnos, &pers, &turn);
        v = arr[0];
        for (i = 1; i < ARRLEN; i++) {
        if (arr[i] != v)
        break;
        }
        if (i < ARRLEN)
        printf("Lector %d, error de lectura\n", num);
        else
        printf("Lector %d, dato %d\n", num, v); 
        read_unlock(&mutex, &no_personas_sala, &pers, &turn);
    }
return NULL;
}
int main() {
pthread_t lectores[M], escritores[N];
pthread_mutex_init(&mutex, NULL);
pthread_cond_init(&no_personas_sala, NULL);

for(int i = 0; i < N + N; i++){
    pthread_cond_init(&cond_turnos[i], NULL);
}

turn.numero = -1;
turn.turno = 0;
turn.total = M + N;

int i;
for (i = 0; i < N; i++)
pthread_create(&escritores[i], NULL, escritor, i + (void*)0);

for (i = 0; i < M; i++)
pthread_create(&lectores[i], NULL, lector, i + (void*)0);
pthread_join(lectores[0], NULL); /* Espera para siempre */
return 0;
}