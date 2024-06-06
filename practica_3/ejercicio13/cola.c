#include "cola.h"
#include <stdio.h>
#include <stdlib.h>


Cola cola_crea() {
    Cola newCola = malloc(sizeof(struct _Nodo));
    newCola->semaforo = NULL;
    newCola->sig = NULL;
    return newCola;
} 

Cola cola_insert(Cola cola, void* dato) {
    if (cola == NULL) {
        Cola newCola = malloc(sizeof(Cola));
        newCola->semaforo = dato;
        newCola->sig = NULL;
        return newCola;
    } else if (cola->sig == NULL) {
        if (cola->semaforo == NULL) {
            cola->semaforo = dato;
            return cola;
        } else {
            Cola newCola = malloc(sizeof(Cola));
            newCola->semaforo = dato;
            newCola->sig = NULL;
            cola->sig = newCola;
            return cola;
        }
    } else {
        cola->sig = cola_insert(cola->sig, dato);
        return cola;
    }
}

void* cola_top(Cola cola) {
    if (cola == NULL)
        return NULL;
    return cola->semaforo;
}

Cola cola_pop(Cola cola, FuncionDestructora funcion) {
    if (cola == NULL)
        return NULL;
    return cola->sig;
}
