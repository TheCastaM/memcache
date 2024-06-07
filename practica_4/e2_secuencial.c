#include <stdio.h>
#include <time.h>
#include <math.h>

int suma = 0;

int main () {
    int inicio = clock();
    double N = 5 * pow(10,8);
    for (int i = 0; i < N; i++) {
        suma = suma + i;
    }
    int fin = clock();

    // Calcula el tiempo transcurrido en segundos
    double tiempo_transcurrido = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    printf("Tiempo transcurrido: %.6f segundos\n", tiempo_transcurrido);
    printf("Suma = %d\n", suma);
    return 0;
}