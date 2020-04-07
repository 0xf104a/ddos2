#ifndef CACHE_H
#define CACHE_H
#include <stdint.h>
#include "array.h"

typedef struct{
    void* data;
} cache_object_t;

typedef struct{
    array_t* cached; 
} cache_t;

void cache_init(void);
size_t cache_put(void* obj);
void* cache_get(size_t id);
#endif
