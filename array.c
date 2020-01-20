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
