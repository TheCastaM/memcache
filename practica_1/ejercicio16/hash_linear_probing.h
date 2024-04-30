#ifndef __HASH_LINEAR_PROBING_H__
#define __HASH_LINEAR_PROBING_H__

#define FACTOR_CARGA_MAXIMO 0.7

typedef void (*FuncionVisitante)(void* dato);
typedef void* (*FuncionCopiadora)(void* dato);
typedef void (*FuncionDestructora)(void* dato);
typedef int (*FuncionComparadora)(void* dato1, void* dato2);

typedef unsigned int (*FuncionHash)(void* dato); 

typedef struct _TablaHash *TablaHash;

TablaHash tablahash_crear(unsigned int size, FuncionCopiadora copy, 
                          FuncionDestructora destroy, FuncionComparadora comp, 
                          FuncionHash hash);

int tablahash_nelems(TablaHash tabla);

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla);

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla);

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 */
void tablahash_insertar(TablaHash tabla, void *dato);

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *tablahash_buscar(TablaHash tabla, void *dato);

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato);

void tablahash_imprimir(TablaHash tabla, FuncionVisitante visit); 

#endif /* __HASH_LINEAR_PROBING_H__ */
