#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "keyvalue.h"

void* keyvalue_copiar(void* dato) {
    KeyValue* kv = dato;
    KeyValue* newKV = malloc(sizeof(KeyValue));
    newKV->key = malloc(sizeof(char)*20);
    newKV->value = malloc(sizeof(char)*20);
    strcpy(newKV->key, kv->key);
    strcpy(newKV->value, kv->value);

    return newKV;
}

void keyvalue_destruir(void* dato) {
    KeyValue* kv = dato;
    free(kv->key);
    free(kv->value);
    free(kv);
}

int keyvalue_comparar(void* dato1, void* dato2) {
    KeyValue* kv1 = dato1;
    KeyValue* kv2 = dato2;
    return (strcmp(kv1->key,kv2->key));
}

int keyvalue_hash(void* dato) {
    KeyValue* kv = dato;
    return KRHash(kv->key);
}