//
//  arguments.h
//  ddos2
//
//  Created by Andre Zay on 16/01/2020.
//  Copyright © 2020 Andre Zay. All rights reserved.
//

#ifndef arguments_h
#define arguments_h

#include <stdbool.h>
#include <stdint.h>

#include "hashtable.h"

typedef enum{
    ARG_INT,
    ARG_STR,
    ARG_BOOL
} argtype;

typedef union{
    int64_t intValue;
    char* strValue;
    bool  boolValue;
}argvalue;

typedef struct{
    char* name;
    char* description;
    bool compulsory;
    argtype type;
    argvalue value;
    bool is_set;
} argument_t;

extern hashtable* arguments;
#endif /* arguments_h */
