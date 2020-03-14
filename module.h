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

#include <stdint.h>

/* Macroses only for use inside of load_module() */
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
    module->NAME=(char*)malloc((strlen(VAR)+1)*sizeof(char)); \
    strcpy(module->NAME, VAR);

#define _MOD_SUMMARY_PRINT(SECTION,PARAM_NAME) \
    printf("%s%s",BOLD,UNDERLINE); \
    printf(SECTION); \
    printf("%s\n",ENDC); \
    printf("%s\n",module->PARAM_NAME);

/* Structures */

typedef struct{
    char* name;
    char* author;
    char* description;
    char* version;
} module_config_t;

typedef struct{
    hashtable* arguments;
    hashtable* network_ifaces;
    bool net_stats;
    const char* version;
    uint8_t* log_byte;
} program_config_t;

typedef struct{
    void* handle;
    char* name;
    char* author;
    char* description;
    char* version;
    char* filename;
    module_config_t* (*mod_on_load)(program_config_t* config);
    void (*mod_on_init)(void);
    void (*mod_on_run)(void);
} module_t;

/* Function prototypes */
void modules_begin(void);
void modules_configure(const char* version);
module_t* module_load(char* path);
void module_summary(module_t* module);
void modules_load(char* path);
void modules_list(void);
module_t* module_get(char* name);
void modules_on_init(void);

#endif /* module_h */
