/**
 *  Es facil ver que estan mal las secuencias.
 * 
 * pthread_mutex_lock(&lk);
 * P = true;
 * pthread_cond_signal(&cv);
 * M = true;
 * pthread_mutex_unlock(&lk); 
 * 
 * 
 *  Lo que sucede es que se libera el proceso
 * A en la linea 'pthread_cond_signal(&cv);' 
 * pero todavia no esta en true la manteca
 * por lo que el proceso A va a destruir la
 * casa.
*/