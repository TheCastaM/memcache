#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define M 5
#define N 5
#define SZ 8

pthread_mutex_t lock; 
pthread_cond_t novacio;
pthread_cond_t nolleno;


/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];
int producidos = 0;

void enviar(int *p)
{
	pthread_mutex_lock(&lock);
    while(producidos >= SZ) {
        pthread_cond_wait(&nolleno, &lock);
    }
    buffer[producidos] = p;
    producidos++;
    pthread_cond_signal(&novacio);
    pthread_mutex_unlock(&lock);
	return;
}

int * recibir()
{
	pthread_mutex_lock(&lock);
    while(producidos == 0) {
        pthread_cond_wait(&novacio, &lock);
    }
    producidos--;
    int* p = buffer[producidos];
    pthread_cond_signal(&nolleno);
    pthread_mutex_unlock(&lock);
	return p;
}

void * prod_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);

		int *p = malloc(sizeof *p);
		*p = random() % 100;
		printf("Productor %d: produje %p->%d\n", id, p, *p);
		enviar(p);
	}
	return NULL;
}

void * cons_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);

		int *p = recibir();
		printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);
		free(p);
	}
	return NULL;
}

int main()
{
	pthread_t productores[M], consumidores[N];
	int i;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&novacio, NULL);
    pthread_cond_init(&nolleno, NULL);

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}
