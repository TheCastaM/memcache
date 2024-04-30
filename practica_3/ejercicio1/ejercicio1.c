/**
 *  Es facil ver que esta bien la secuencia.
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
 * pero no puede actuar hasta tener el lock, 
 * por lo que aunque M sea false no rompe la
 * casa, sino que espera a que se libere y ahi
 * arranca.
*/