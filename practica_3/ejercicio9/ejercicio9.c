#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "barrier.h"

#define Green "\033[0;32m"
#define Red "\033[0;31m"
#define End "\033[0m"

#define N 10
#define ITERS 4
#define W 5
#define min(i, j) (((i) < (j)) ? (i) : (j))

barrier barrera;
pthread_t hilos[W];

float arr1[N], arr2[N];

void calor(float *arr, int lo, int hi, float *arr2) {
    int i;
    for (i = lo; i < hi; i++) {
        int m = arr[i];
        int l = i > 0 ? arr[i-1] : m;
        int r = i < N-1 ? arr[i+1] : m;
        arr2[i] = m + (l - m)/1000.0 + (r - m)/1000.0;
    }
}

/* Dado un array de [n], devuelve el punto de corte [i] de los [m] totales. */

static inline int cut(int n, int i, int m) {
    return i * (n/m) + min(i, n %m);
}

void * thr(void *arg) {
    int id = arg - (void*)0; /* 0 <= id < W */
    int lo = cut(N, id, W), hi = cut(N, id+1, W);
    int i;
    for (i = 0; i < ITERS; i++) {
        calor(arr1, lo, hi, arr2);
        printf(Red"Termine el calor 1\n"End);
        barrier_wait(&barrera);

        calor(arr2, lo, hi, arr1);
        printf(Green"Termine el calor 2\n"End);
        barrier_wait(&barrera);
    }
}

int main() {


    for (int i = 0; i < N; i++) {
        arr1[i] = rand() % N;
        arr2[i] = rand() % N;
    }

    barrier_init(&barrera, W);
    
    for(int i = 0; i < W; i++ ) {
        pthread_create(&hilos[i], NULL, thr, i + (void*)0);
    }

    pthread_join(hilos[0], NULL);
    
    return 0;
}

