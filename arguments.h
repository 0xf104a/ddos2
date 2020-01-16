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

void arguments_begin(void);
argument_t *argument_create(char* name, char* description, argtype type, bool compulsory, argvalue _default,bool has_default_value);
void argument_add_compulsory(char* name, char* description, argtype type);
void argument_add(char* name, char* description, argtype type, argvalue _default, bool has_default_value);
void arguments_parse(int argc, const char * argv[], int start);
argument_t* argument_get(char *name);
bool argument_check(char *name);
argvalue argument_value_get(char *name)
#endif /* arguments_h */
