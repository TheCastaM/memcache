#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <time.h>

int suma = 0;

int main () {
    int inicio = clock();
    double N = 5 * pow(10,8);
    int i;
    #pragma omp parallel num_threads(4) 
    {   
        int id = omp_get_thread_num() ;
        int nt = omp_get_num_threads() ;
        printf("id: %d nt: %d\n", id ,nt);
        #pragma omp for reduction (+ : suma)
            for(i =  0; i < (int)N; i++) {
                suma = suma + i;        
            }
 
    }
    int fin = clock();

    // Calcula el tiempo transcurrido en segundos
    double tiempo_transcurrido = ((double)(fin - inicio)) / CLOCKS_PER_SEC;

    printf("Tiempo transcurrido: %.6f segundos\n", tiempo_transcurrido);

    printf("Suma = %d\n", suma);
}



