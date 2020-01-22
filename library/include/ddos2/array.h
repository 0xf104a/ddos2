//
//  array.h
//  ddos2
//
//  Created by Andre Zay on 08/07/2019.
//  Copyright © 2019 Andre Zay. All rights reserved.
//

#ifndef array_h
#define array_h

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct{
    size_t sz;
    size_t capacity;
    void **base;
} array_t;

array_t *array_create(int capacity);
void array_add(array_t *_array,void *object);
void array_free(array_t *_array);
bool in_array(array_t *_array, void *object);

#endif /* array_h */
