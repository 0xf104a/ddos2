//
//  module.h
//  ddos2
//
//  Created by Andre Zay on 16/01/2020.
//  Copyright © 2020 Andre Zay. All rights reserved.
//

#ifndef module_h
#define module_h

#include "array.h"

typedef struct{
    array_t *arguments;
} module_data_t;
typedef struct{
    void *handle;
    
} module_t;

#endif /* module_h */
