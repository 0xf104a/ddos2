#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "hashtable.h"

char* mod_name="mod_a";
char* mod_author="Anrewerr@github.com";
char* mod_version="<none>";
char* mod_description="Useless module for debugging.";

typedef struct{
    hashtable* arguments;
    const char* version;
    uint8_t* log_byte;
} program_config_t;

typedef struct{
    array_t *arguments;
    char* name;
    char* author;
    char* description;
    char* version;
    //char* filename;
} module_config_t;

module_config_t* mod_config_pull(program_config_t* config){
   
 //  printf("config@%d\n\n",config);
   printf("Arguments addr:%d\n",config->arguments);
  
   printf("Program version:%s\n",config->version);
    printf("Log byte:%d\n",*config->log_byte);
   module_config_t* cfg=(module_config_t*)malloc(sizeof(module_config_t));
   cfg->arguments=array_create(1);
   cfg->name=(char*)malloc(sizeof(char)*strlen(mod_name)+sizeof(char));
   cfg->author=(char*)malloc(sizeof(char)*strlen(mod_author)+sizeof(char));
   cfg->description=(char*)malloc(sizeof(char)*strlen(mod_description)+sizeof(char));
   cfg->version=(char*)malloc(sizeof(char)*strlen(mod_version)+sizeof(char));
   strcpy(cfg->name,mod_name);
   strcpy(cfg->author,mod_author);
   strcpy(cfg->description,mod_description);
   strcpy(cfg->version,mod_version);
   return cfg;
}
void mod_on_init(void){
   printf("mod_on_init() called.\n");
}

