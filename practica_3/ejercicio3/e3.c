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
    pthread_mutex_lock(der(i));
    pthread_mutex_lock(izq(i));
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
Deadlock: Situacion donde un proceso A requiere un recurso que
tiene un proceso B para avanzar, pero este proceso B necesita un
recurso que tiene A para avanzar, por lo que se quedan bloqueados.

a) Este programa termina en deadlock si cada filosofo toma su
tenedor derecho de forma consecutiva (es decir, cada uno tomo su
tenedor derecho sin tomar el izquierdo). El deadlock se genera
ya que cada filosofo requerira un tenedor izquierdo para poder
comer y liberar sus tenedores, pero esto nunca pasa ya que para
cada filosofo su tenedor izquierdo estara tomado. Cada filosofo
necesita un recurso que ya esta tomado para liberarse.

*/