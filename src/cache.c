#include "cache.h"

#include "config.h"
#include "hashtable.h"
#include "array.h"
#include "message.h"

//TODO: Thread security
cache_t* cache;
void cache_init(void){
    cache=(cache_t*)malloc(sizeof(cache_t));
}
size_t cache_put(void* obj){
    array_add(cache->cached,obj);
    return cache->cached->sz-1;
}
void* cache_get(size_t id){
    return cache->cached->base[id];
}

