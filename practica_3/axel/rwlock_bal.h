#include <pthread.h>
#include <semaphore.h>

typedef struct {
    int numero;
    int turno;
    int total;
}Turnos;

void write_lock(pthread_mutex_t* m , pthread_cond_t* c, pthread_cond_t* c2, int* pers, Turnos* turnos);

void write_unlock(pthread_mutex_t* m, pthread_cond_t* c2, Turnos* turnos);

void read_lock(pthread_mutex_t* m,pthread_cond_t* c2, int* pers, Turnos* turnos);

void read_unlock(pthread_mutex_t* m , pthread_cond_t* c, int* pers, Turnos* turnos);
