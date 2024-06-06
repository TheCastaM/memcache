#include <stdio.h>
#include <omp.h>

#define THREADS 2
#define N_VISITANTES 10000000

/* Variables globales */
int numero[THREADS];
int eligiendo[THREADS];
int visitantes = 0;


int max(int* a) {
    int max = 0;
    for(int i =0; i < THREADS; i++) {
        if(*a > max) max = *a;
        a++;
    }
    return max;
}
/* Código del hilo i */
void lock(int i) {

    #pragma omp critical (cs1)
    {
        eligiendo[i] = 1;
        numero[i] = 1 + max(numero);
        eligiendo[i] = 0;
    } 
    
    for (int j = 0; j < THREADS; j++) {

    while ( eligiendo[j] ) { /* busy waiting */ }

    while ((numero[j] != 0) &&
    ((numero[j] < numero[i]) || ((numero[j] == numero[i]) && (j
    < i)))) { /* busy waiting */ }
    }

}

void unlock(int i) {
    numero[i] = 0;
}

void* molinete(void* arg) {
    int t = arg - (void*)0;

    for(int i = 0; i < N_VISITANTES; i++) {
        lock(t);
        visitantes++;
        unlock(t);
        
    }

    return NULL;
}

int main () {
    #pragma omp parallel num_threads(2)
    {
        int id = omp_get_thread_num();
        molinete(id + (void*)0);
    }
    printf("Visitantes : %d\n", visitantes);
}