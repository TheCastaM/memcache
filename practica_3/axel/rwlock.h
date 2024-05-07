
#include <pthread.h>

void write_lock(pthread_mutex_t* m, pthread_cond_t* c, int* pers);

void write_unlock(pthread_mutex_t* m, int* pers);

void read_lock(pthread_mutex_t* m, pthread_cond_t* c, int* pers);

void read_unlock(pthread_mutex_t* m, pthread_cond_t* c, int* pers);