#ifndef THREADING_H
#define THREADING_H

#include <pthread.h>

extern pthread_mutex_t mutex;

void lock_mutex(void);
void unlock_mutex(void);

#endif
