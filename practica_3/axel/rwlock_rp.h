#include <pthread.h>

void write_lock(pthread_mutex_t* m, pthread_cond_t* c, int* pers, int* w);

void write_unlock(pthread_mutex_t* m, pthread_cond_t* c, int* w);

void read_lock(pthread_mutex_t* m, pthread_cond_t* c, int* pers, int* w);

void read_unlock(pthread_mutex_t* m, pthread_cond_t* c, int* pers);