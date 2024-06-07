#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>




int suma = 0;
pthread_mutex_t lock;

int main () {
    int inicio = clock();
    double N = 5 * pow(10,8);
    pthread_mutex_init(&lock, NULL);
    int i;
    #pragma omp parallel num_threads(2) 
    {   
        #pragma omp for     
            for(i =  0; i < (int)N; i++) {
                pthread_mutex_lock(&lock);
                suma = suma + i;        
                pthread_mutex_unlock(&lock);
            }
    }
    int fin = clock();

    // Calcula el tiempo transcurrido en segundos
    double tiempo_transcurrido = ((double)(fin - inicio)) / CLOCKS_PER_SEC;

    printf("Tiempo transcurrido: %.6f segundos\n", tiempo_transcurrido);

    printf("Suma = %d\n", suma);
}
