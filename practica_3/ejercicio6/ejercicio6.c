#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define Green "\033[0;32m"
#define Red "\033[0;31m"
#define End "\033[0m"

#define SILLAS 5
#define MAX_CLIENTES 10

sem_t atendido;
sem_t durmiendo;
sem_t ocupado;

pthread_t barber;
pthread_t clientes[MAX_CLIENTES];

pthread_mutex_t lock;

int sillas = SILLAS;

void cortando() {
    printf(Red"Cortando...\n"End);
}

void pagando() {
    printf(Green"Pagando...\n"End);
}

void me_pagan() {
    printf(Red"Listo. Me pagan...\n"End);
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
        printf(Green"Me siento...\n"End);
        sillas--;
        return 1;
    }
    else{
        printf(Green"No hay sillas, me voy...\n"End);
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
    
    return 0;
}


/*
Comportamiento cliente:
Entro y me fijo si hay sillas:
    si hay silla = me siento
    si no hay silla = me voy

Cuando el barbero esta ocupado, le esta cortando el pero a
alguien. Espero a que este desocupado para ver si me corta
a mi.

Hasta que el barbero no termine de cortar y cobrar, no se libera
una silla ni deja de estar ocupado
*/