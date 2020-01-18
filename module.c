//
//  module.c
//  ddos2
//
//  Created by Andre Zay on 16/01/2020.
//  Copyright © 2020 Andre Zay. All rights reserved.
//

#include "module.h"

#include "message.h"
#include "config.h"
#include "arguments.h"

#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

hashtable* modules;

void modules_begin(void){
    modules=hashtbl_create(HASHTBL_CAPACITY);
}
void module_register_arguments(array_t* _arguments){
    int i=0;
    for(;i<arguments->sz;++i){
        _register_argument((argument_t*)arguments->base[i]);
    }
}
module_t* module_load(char* path){
    char* _error;
    module_t *module=(module_t*)malloc(sizeof(module));
    module->handle=dlopen(path, RTLD_LAZY);
    if (!module->handle) {
        warn("Loading module %s failed: %s",path,dlerror());
        return NULL;
    }
    /*Load functions*/
    _MOD_IMPORT_FUNCTION("mod_config_pull", mod_pull_config);
    _MOD_IMPORT_FUNCTION("mod_config_push", mod_push_config);
    /*Load other symbols*/
    _MOD_IMPORT_SYMBOL(const char*, "mod_name", mod_name);
    _MOD_IMPORT_SYMBOL(const char*, "mod_author", mod_author);
    _MOD_IMPORT_SYMBOL(const char*, "mod_version", mod_version);
    _MOD_IMPORT_SYMBOL(const char*, "mod_description", mod_description);
    /*Set module params*/
    _MOD_STR_SET(name, mod_name);
    _MOD_STR_SET(author, mod_author);
    _MOD_STR_SET(version, mod_version);
    _MOD_STR_SET(description, mod_description);
    /*Configure*/
    module_config_t* config=module->mod_pull_config();
    module_register_arguments(config->arguments);
    /*Register module*/
    hashtbl_add(modules, module->name, module);
    
    return module;
}
