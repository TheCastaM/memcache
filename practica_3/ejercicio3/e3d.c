#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define N_FILOSOFOS 5
#define ESPERA 5000000
pthread_mutex_t tenedor[N_FILOSOFOS];
pthread_mutex_t * izq(int i) { return &tenedor[i]; }
pthread_mutex_t * der(int i) { return &tenedor[(i+1) % N_FILOSOFOS]; }

void pensar(int i) {
    printf("Filosofo %d pensando...\n", i);
    usleep(random() % ESPERA);
}
void comer(int i) {
    printf("Filosofo %d comiendo...\n", i);
    usleep(random() % ESPERA);
}
int tomar_tenedores(int i) {
    pthread_mutex_lock(der(i));
    if(pthread_mutex_trylock(izq(i)) != 0) {
        pthread_mutex_unlock(der(i));
        return 0;
    }
    else  {
        return 1;
    } 
}
void dejar_tenedores(int i) {
    pthread_mutex_unlock(der(i));
    pthread_mutex_unlock(izq(i));
}
void * filosofo(void *arg) {
    int i = arg - (void*)0;
    while (1) {
        if(tomar_tenedores(i)) {
            comer(i);
            dejar_tenedores(i);
            pensar(i);        
        }
        else {
            printf("Filosofo %d esperando...\n", i);
            usleep(random() % ESPERA);
        }
    }
}
int main() {
    pthread_t filo[N_FILOSOFOS];
    int i;
    for (i = 0; i < N_FILOSOFOS; i++)
    pthread_mutex_init(&tenedor[i], NULL);
    for (i = 0; i < N_FILOSOFOS; i++)
    pthread_create(&filo[i], NULL, filosofo, i + (void*)0);
    pthread_join(filo[0], NULL);
    return 0;
}

/*
No hay deadlock, ya que si lo hubiera, todos tendrian un tenedor y estarian esperando otro.
Suponiendo que tienen el derecho, estan esperando el izquierdo, y como no lo pudieron obtener,
liberan el derecho que poseen, con lo cual su vecino de la derecha podra comer y entonces no hay deadlock

*/