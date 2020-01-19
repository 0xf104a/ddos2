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
#include "commons.h"

#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

hashtable* modules;
program_config_t *p_config;
void modules_begin(void){
    modules=hashtbl_create(HASHTBL_CAPACITY);
    p_config=(program_config_t*)malloc(sizeof(program_config_t));
}

void modules_configure(const char* version){
    p_config->arguments=arguments;
    p_config->log_byte=&log_level;
    p_config->version=version;
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
    _MOD_IMPORT_FUNCTION("mod_on_init", mod_on_init);
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
    _MOD_STR_SET(filename, path);
    /*Configure*/
    module_config_t* config=module->mod_pull_config(p_config);
    module_register_arguments(config->arguments);
    /*Register module*/
    hashtbl_add(modules, module->name, module);
    
    debug("Loaded: %s",path);
    
    return module;
}

void module_summary(module_t *module){
    printf("%sMODULE NAME:%s %s\n",BOLD,ENDC,module->name);
    _MOD_SUMMARY_PRINT("VERSION", version);
    _MOD_SUMMARY_PRINT("AUTHOR", author);
    _MOD_SUMMARY_PRINT("DESCRIPTION", description);
    _MOD_SUMMARY_PRINT("FILENAME", filename);
}

void modules_load(char* path){
    if(!modules){
        error("Programming error: modules==NULL! Have you called modules_begin()?");
        return ;
    }
    DIR* directory;
    struct dirent* file;
    directory=opendir(path);
    if(directory==NULL){
        die("Could not open modules directory: %s(%d).",strerror(errno),errno);
    }
    file=readdir(directory);
    //FIXME: Incorrect path to file: need to concat modules path and module name
    while (file!=NULL) {
        if(is_regular_file(file->d_name)){
            debug("Loading module:%s",file->d_name);
            module_load(file->d_name);
        }else{
            debug_warn("Omitting non regular file:%s",file->d_name);
        }
        file=readdir(directory);
    }
}
