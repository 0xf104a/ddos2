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
#include "hashtable.h"

/*Macroses only for use inside of load_module()*/
#define _MOD_IMPORT_SYMBOL(TYPE, NAME, VAR_NAME) \
    TYPE VAR_NAME = *(TYPE *)dlsym(module->handle, NAME); \
    _error=dlerror(); \
    if(_error!=NULL){ \
        error("Failed to get module symbol from %s: %s",path, _error); \
        free(module); \
        return NULL; \
    }

#define _MOD_IMPORT_FUNCTION(NAME, ATTRIB_NAME) \
    module->ATTRIB_NAME=dlsym(module->handle, NAME); \
    _error=dlerror(); \
    if(_error!=NULL){ \
        error("Failed to get module function from %s: %s",path, _error); \
        free(module); \
        return NULL; \
    }

#define _MOD_STR_SET(NAME, VAR) \
    module->NAME=malloc(strlen(VAR)*sizeof(char)); \
    strcpy(module->NAME, VAR);

#define _MOD_SUMMARY_PRINT(SECTION,PARAM_NAME) \
    printf("%s%s",BOLD,UNDERLINE); \
    printf(SECTION); \
    printf("%s\n",ENDC); \
    printf("%s\n",module->PARAM_NAME);

typedef struct{
    array_t *arguments;
} module_config_t;

typedef struct{
    hashtable *arguments;
    const char *version;
} program_config_t;

typedef struct{
    void *handle;
    char *name;
    char *author;
    char *description;
    char *version;
    module_config_t* (*mod_pull_config)(void);
    void (*mod_push_config)(program_config_t *config);
} module_t;

#endif /* module_h */
