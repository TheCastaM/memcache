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
void tomar_tenedores(int i) {
    if (i == 0) {
        pthread_mutex_lock(izq(i));
        pthread_mutex_lock(der(i));
    }
    else {
        pthread_mutex_lock(der(i));
        pthread_mutex_lock(izq(i));
    }
}
void dejar_tenedores(int i) {
    pthread_mutex_unlock(der(i));
    pthread_mutex_unlock(izq(i));
}
void * filosofo(void *arg) {
    int i = arg - (void*)0;
    while (1) {
    tomar_tenedores(i);
    comer(i);
    dejar_tenedores(i);
    pensar(i);
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
Esta solucion funciona ya que :
Si el filosofo A que toma a izquierda no puede tomar a derecha, es 
porque el filosofo B que esta a derecha tomo este tenedor, que seria
el izquierdo de B y por lo tanto pudo comer (ya que cualquiera
que no sea A toma primero a derecha y luego a izquierda), y ents
el filosofo A podra comer luego de que este termine.

Si el filosofo A que toma a izquierda no puede tomar a izquierda,
entonces el filosofo B que esta su derecha podra eventualmente
tomar el tenedor a izquierda, por lo que no se produce un deadlock

*/