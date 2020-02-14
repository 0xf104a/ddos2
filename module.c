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
#include "network.h"

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
    p_config->network_ifaces=network_ifaces;
    p_config->net_stats=network_statistics;
}

void module_register_arguments(array_t* _arguments){
    size_t i=0;
    for(;i<_arguments->sz;i++){
        _register_argument((argument_t*)_arguments->base[i]);
    }
}

module_t* module_load(char* path){
    char* _error;
    module_t *module=(module_t*)malloc(sizeof(module_t));
    module->handle=dlopen(path, RTLD_LAZY);
    if (!module->handle) {
        error("Loading module %s failed: %s",path,dlerror());
        return NULL;
    }
    /*Load functions*/
    _MOD_IMPORT_FUNCTION("mod_on_load", mod_on_load);
    _MOD_IMPORT_FUNCTION("mod_on_init", mod_on_init);

    /*Configure*/
    module_config_t* config=(module->mod_on_load)(p_config);
    if(!config){
        error("Programming error: config==NULL!Check that your module work properly.Called function: mod_on_load(). File: %s.(%s:%d)",path,__FILE__,__LINE__);
        return NULL;
    }
    /* Set params */
    _MOD_STR_SET(name, config->name);
    _MOD_STR_SET(author, config->author);
    _MOD_STR_SET(version, config->version);
    _MOD_STR_SET(description, config->description);
    _MOD_STR_SET(filename, path);
    /*Register module*/
    hashtbl_add(modules, module->name, module);

    debug("Loaded: %s",path);
    
    /* Free config since it used only on module initialization */
    free(config->author);
    free(config->description);
    free(config->name);
    free(config->version);
    free(config);
    
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
    //FIXED: Incorrect path to file: need to concat modules path and module name
    while (file!=NULL) {
        /* Get full filename */
        char* fname=(char*)malloc(sizeof(char)*(strlen(MODULES_DIR)+strlen(file->d_name)+2));
        strcpy(fname, MODULES_DIR);
        strcat(fname, file->d_name);
        /* Load module if file was found */
        if(is_regular_file(fname)){
            debug("Loading module:%s",fname);
            if(!module_load(fname)){
                warn("Module %s was not loaded!", fname);
            }
        }else{
            debug_warn("Omitting non-regular file:%s",fname);
        }
        file=readdir(directory);
    }
}

void modules_list(void){
    int i=0;
    printf("%-25s%-10s%-20s\n", "Name", "Version", "Author");
    for(;i<modules->sz;++i){
        module_t* module = (module_t*)modules->values->base[i];
        printf("%-25s%-10s%-20s\n", module->name, module->version, module->author);
    }
}

bool module_loaded(char* name){
    return hashtbl_check_key(modules, name);
}

module_t* module_get(char* name){
    if(!module_loaded(name)){
        error("Programming error: module %s was not loaded!(%s:%d)",__FILE__,__LINE__);
    }
    return hashtbl_get(modules, name);
}

void modules_on_init(void){
    int i=0;
    for(;i<modules->sz;++i){
        module_t* module = (module_t*)modules->values->base[i];
        module->mod_on_init();
    }
}

