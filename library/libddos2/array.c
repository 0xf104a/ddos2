//
//  array.c
//  ddos2
//
//  Created by Andre Zay on 08/07/2019.
//  Copyright © 2019 Andre Zay. All rights reserved.
//

#include "array.h"

#include "message.h" //For debug


#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

array_t *array_create(int capacity){
    array_t *_array=(array_t *)malloc(sizeof(array_t));
    _array->capacity=capacity;
    _array->sz=0;
    _array->base=(void **)malloc(sizeof(void*)*capacity);
    _array->base[0]=NULL;
    return _array;
}

void array_add(array_t *_array,void *object){
     if(_array->sz+1 >= _array->capacity){
         _array->base=(void**)realloc(_array->base, sizeof(void*)*(_array->capacity+1));
         _array->capacity++;
     }
    _array->base[_array->sz]=object;
    _array->sz++;
}

void array_free(array_t *_array){
    free(_array->base);
    free(_array);
}

bool in_array(array_t *_array, void *object){
    int i=0;
    for(;i<_array->sz;++i){
        if(object==_array->base[i]){
            return true;
        }
    }
    return false;
}

void array_pop(array_t *_array){
    if(_array->sz<=1){
        return ;
    }
    _array->base=(void**)realloc(_array->base, (_array->sz-1)*sizeof(void*));
    _array->sz--;
}

void array_cat(array_t *dst,array_t *src){
    int i=0;
    for(;i<src->sz;++i){
        array_add(dst, src->base[i]);
    }
}

array_t *array_cpy(array_t *src){
    array_t *_new=array_create((int)src->sz);
    memcpy(_new->base, src->base, sizeof(void*)*src->sz);
    _new->sz=src->sz;
    return _new;
}

void array_del(array_t *_array,int pos){
    assert(pos<_array->sz);
    if(pos==_array->sz-1){
        array_pop(_array);
        return ;
    }
    int i=pos+1;
    for(;i<_array->sz;i++){
        _array->base[i-1]=_array->base[i];
    }
    array_pop(_array);
}
