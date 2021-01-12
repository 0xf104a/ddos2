#include "threading.h"

#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void lock_mutex(void){
     pthread_mutex_lock(&mutex);
}

void unlock_mutex(void){
     pthread_mutex_unlock(&mutex);
}


