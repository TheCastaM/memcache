#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define M 5
#define N 5
#define SZ 8

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];
int counter = 0;
sem_t prod;
sem_t disp;
pthread_mutex_t mutex;


void enviar(int *p)
{
    sem_wait(&prod);
    pthread_mutex_lock(&mutex);
    buffer[counter] = p;
    counter++;
    pthread_mutex_unlock(&mutex);
	sem_post(&disp); /*aviso que hay uno mas disponible*/
	return;
}

int * recibir()
{
	sem_wait(&disp); /*aviso que hay uno menos disponible*/
    pthread_mutex_lock(&mutex);
	counter--;
	//printf("%d\n", counter);
    int* p = buffer[counter];
    pthread_mutex_unlock(&mutex);
	sem_post(&prod);
	return p;
}

void * prod_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);

		int *p = malloc(sizeof *p);
		*p = random() % 100;
		printf("\033[0;31mProductor %d\033[0m: produje %p->%d\n", id, p, *p);
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
		printf("\033[0;32mConsumidor %d\033[0m: obtuve %p->%d\n", id, p, *p);
		free(p);
	}
	return NULL;
}

int main()
{
	pthread_t productores[M], consumidores[N];
	int i;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&prod, 0, SZ);
    sem_init(&disp, 0, 0);

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}
