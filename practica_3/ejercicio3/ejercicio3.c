#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define N_FILOSOFOS 5
#define ESPERA 5000000

pthread_mutex_t tenedor[N_FILOSOFOS];
pthread_mutex_t * izq(int i) { return &tenedor[i]; }
pthread_mutex_t * der(int i) { return &tenedor[(i+1) % N_FILOSOFOS]; }

void pensar(int i)
{
    printf("Filosofo %d pensando...\n", i);
    usleep(random() % ESPERA);
}

void comer(int i)
{
    printf("Filosofo %d comiendo...\n", i);
    usleep(random() % ESPERA);
}

void tomar_tenedores(int i)
{
    if (i == 0) {
        pthread_mutex_lock(izq(i));
        pthread_mutex_lock(der(i));
    } else {
        pthread_mutex_lock(der(i));
        pthread_mutex_lock(izq(i));
    }
}

void dejar_tenedores(int i)
{
    pthread_mutex_unlock(der(i));
    pthread_mutex_unlock(izq(i));
}

void * filosofo(void *arg)
{
    int i = arg - (void*)0;
    while (1) 
    {
        tomar_tenedores(i);
        comer(i);
        dejar_tenedores(i);
        pensar(i);
    }
}

int main()
{
    pthread_t filo[N_FILOSOFOS];
    
    int i;
    
    for (i = 0; i < N_FILOSOFOS; i++)
        pthread_mutex_init(&tenedor[i], NULL);

    for (i = 0; i < N_FILOSOFOS; i++)
        pthread_create(&filo[i], NULL, filosofo, i + (void*)0);
    
    pthread_join(filo[0], NULL);
    
    return 0;
}

/**
 *  a) Podemos caer en el caso de deadlock porque 
 * si todos los procesos agarran a la vez su 
 * tenedor derecho y no llegan a agarrar el 
 * izquierdo todos se quedan esperando y no comen.
 * Como no comen no liberan el tenedor y entonces
 * quedamos en un momento donde todos esperan y 
 * nadie avanza.
 * 
 *  b) Ponemos que el comensal 0 es zurdo porque
 * sabemos que siempre va a haber un comensal. Si
 * este toma el tenedor izquierdo entonces sabemos
 * que va a poder tomar el derecho porque nadie 
 * aparte de el toma el tenedor izquiero y a partir
 * de ahi no podemos caer en un deadlock. Si este
 * no llega a tomar el tenedor izquierdo entonces
 * tampoco toma el derecho y el que esta a su 
 * derecha va a tomar su tenedor derecho y asi no 
 * caemos en un deadlock.
 *  
*/