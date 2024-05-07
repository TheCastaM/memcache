#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define SILLAS 7
#define MAX_CLIENTES 5

sem_t atendido;
sem_t durmiendo;
sem_t ocupado;

pthread_t barber;
pthread_t clientes[MAX_CLIENTES];

pthread_mutex_t lock;

int sillas = SILLAS;

void cortando() {
    printf("Cortando...\n");
}

void me_pagan() {
    printf("Listo. Me pagan...\n");
    sem_post(&atendido);
}


void* barbero () {
    while(1) {
        sleep(random() % 3);
        sem_wait(&durmiendo);
        cortando();
        me_pagan();
    }
}

int entrar_peluqueria() {
    if(sillas > 0) {
        printf("Me siento...\n");
        sillas--;
        return 1;
    }
    else{
        printf("No hay sillas, me voy...\n");
        return 0;
    } 
}

void salir_peluqueria() {
    sillas++;
}


void me_cortan() {
    sem_post(&durmiendo);
    sem_wait(&atendido);
}

void pagando() {
    printf("Pagando...\n");
}

void* cliente () {
    while(1) {
        sleep(random() % 3);

        pthread_mutex_lock(&lock);
        int entre = entrar_peluqueria();
        pthread_mutex_unlock(&lock);

        if(entre) {
            sem_wait(&ocupado);
            me_cortan();
            pagando();
            salir_peluqueria();
            sem_post(&ocupado);
        }
    }
}

int main() {
    sem_init(&atendido, 0 , 0);
    sem_init(&durmiendo, 0 , 0);
    sem_init(&ocupado, 0 , 1);

    pthread_create(&barber, NULL, barbero, NULL);
    for(int i = 0 ; i < MAX_CLIENTES; i++) {
        pthread_create(&clientes[i], NULL, cliente, NULL);
    }

    pthread_join(barber, NULL);
}


/*


Comportamiento cliente:
Entro y me fijo si el barbero esta ocupado:
Si esta ocupado:
    si hay silla : me siento
    si no hay silla: me voy

Si no esta ocupado, me corto el pelo

Hasta que el barbero no termine de cortar y cobrar, no se libera
una silla ni deja de estar ocupado


            pthread_mutex_lock(&lock);
            pthread_mutex_unlock(&lock);
*/