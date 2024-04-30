#include "hash_linear_probing.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct _CasillaHash {
  void* dato;
  int eliminado;
} CasillaHash;

struct _TablaHash {
  CasillaHash* arr;

  unsigned int size;
  unsigned int cant;

  FuncionCopiadora copy;
  FuncionDestructora destroy;
  FuncionComparadora comp;
  FuncionHash hash;
};

static unsigned int rehash(TablaHash tabla, unsigned int idx) {
  return (idx + 1) % tabla->size;
}

TablaHash tablahash_crear(unsigned int size, FuncionCopiadora copy, 
                          FuncionDestructora destroy, FuncionComparadora comp, 
                          FuncionHash hash) {
  TablaHash tabla = malloc(sizeof(struct _TablaHash));

  tabla->size = size;
  tabla->cant = 0;

  tabla->copy = copy;
  tabla->destroy = destroy;
  tabla->comp = comp;
  tabla->hash = hash;

  tabla->arr = malloc(sizeof(CasillaHash) * size);

  for (unsigned int i = 0; i < size; i ++) {
    tabla->arr[i].dato = NULL;
    tabla->arr[i].eliminado = 0;
  }

  return tabla;
}

int tablahash_nelems(TablaHash tabla) {
  assert(tabla != NULL);
  return tabla->cant;
}

int tablahash_capacidad(TablaHash tabla) {
  assert(tabla != NULL);
  return tabla->size;
}

void tablahash_destruir(TablaHash tabla) {
  assert(tabla != NULL);

  for (unsigned int i = 0; i < tabla->size; i++) {
    if (tabla->arr[i].dato != NULL) {
      tabla->destroy(tabla->arr[i].dato);
    }
  }

  free(tabla->arr);
  free(tabla);
}

static unsigned int tablahash_buscar_idx(TablaHash tabla, void* dato) {
  if (tabla == NULL || dato == NULL) return -1;

  // Calculamos la clave del dato donde deberia estar en la tabla hash
  unsigned int idx = tabla->hash(dato) % tabla->size;

  if (tabla->arr[idx].dato == NULL && tabla->arr[idx].eliminado == 0) {
    return -1;
  }

  else if (tabla->arr[idx].dato != NULL &&
           tabla->comp(tabla->arr[idx].dato, dato) == 0) {
    return idx;
  }

  else {
    
    unsigned int i = idx;
    int encontro = -1;

    do {
      i = rehash(tabla, i);

      if (tabla->arr[i].dato != NULL) {
        encontro = tabla->comp(tabla->arr[i].dato, dato);
      }

    } while(i != idx && encontro != 0);
    
    if (i != idx) return i;

    return -1;
  }
}

void *tablahash_buscar(TablaHash tabla, void *dato) {
  if (tabla == NULL || dato == NULL) return NULL;

  // Calculemos la clave del dato dentro de la tabla, en caso de no estar
  // sera -1
  unsigned int idxIn = tablahash_buscar_idx(tabla, dato);

  if (idxIn == (unsigned int)-1) return NULL;

  return tabla->copy(tabla->arr[idxIn].dato);
}

static void tablahash_redimencionar(TablaHash tabla) {
  assert(tabla != NULL);

  tabla->size = tabla->size * 2;

  CasillaHash *oldArr = tabla->arr;

  tabla->arr = malloc(sizeof(CasillaHash) * tabla->size);

  for (unsigned int idx = 0; idx < tabla->size; idx++) {
    tabla->arr[idx].dato = NULL;
    tabla->arr[idx].eliminado = 0;
  }

  for (unsigned int idx = 0; idx < tabla->size / 2; idx++) {
    if (oldArr[idx].dato != NULL) {
      tablahash_insertar(tabla, oldArr[idx].dato);
      tabla->destroy(oldArr[idx].dato);
    } 
  }

  free(oldArr);
}

void tablahash_insertar(TablaHash tabla, void *dato) {
  assert(tabla != NULL);
  assert(dato != NULL);

  float factorCarga = (float)tabla->cant / (float)tabla->size;

  if (factorCarga >= FACTOR_CARGA_MAXIMO) {
    tablahash_redimencionar(tabla);
  }

  // Calculamos la clave que deberia tener dentro de la tabla hash
  unsigned int idx = tabla->hash(dato) % tabla->size;

  // Busquemos si es que existe la clave del dato dentro de la tabla hash
  // Si la clave no esta sera -1
  unsigned int idxIn = tablahash_buscar_idx(tabla, dato);

  if (idxIn == (unsigned int)-1) {
    for (; tabla->arr[idx].dato != NULL; idx = rehash(tabla, idx));

    tabla->arr[idx].dato = tabla->copy(dato);
    tabla->arr[idx].eliminado = 0;
    tabla->cant++;
  }
  
  else {
    tabla->destroy(tabla->arr[idxIn].dato);
    tabla->arr[idxIn].dato = tabla->copy(dato);
    tabla->arr[idxIn].eliminado = 0;
  }

  return;
}

void tablahash_eliminar(TablaHash tabla, void *dato) {
  if (tabla == NULL || dato == NULL) return;

  // Busquemos el idx (clave) que tiene el dato dentro de la tabla,
  // en caso de no encontrarse esta clave sera -1
  unsigned int idxIn = tablahash_buscar_idx(tabla, dato);

  if (idxIn == (unsigned int)-1) return;

  tabla->destroy(tabla->arr[idxIn].dato);
  tabla->arr[idxIn].eliminado = 1;

  tabla->arr[idxIn].dato = NULL;
  tabla->cant--;
}

void tablahash_imprimir(TablaHash tabla, FuncionVisitante visit) {
  puts(""); 
  for (unsigned int idx = 0; idx < tabla->size; idx++) {
    printf("DATO: ");
    if (tabla->arr[idx].dato != NULL) {
      visit(tabla->arr[idx].dato);
    }
    printf(" CLAVE: %d", idx);
    puts("");
  }
}
