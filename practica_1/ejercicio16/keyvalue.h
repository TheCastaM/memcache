typedef struct {
    char* key;
    char* value;
} KeyValue;

void* keyvalue_copiar(void* dato);

void keyvalue_destruir(void* dato);

int keyvalue_comparar(void* dato1, void* dato2);

int keyvalue_hash(void* dato);