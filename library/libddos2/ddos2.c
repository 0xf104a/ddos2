#include "ddos2.h"
#include "message.h"
#include "arguments.h"
#include "network.h"

#include <string.h>
#include <stdlib.h>

void ddos2_begin(program_config_t* config){
    arguments_begin(config->arguments);
    message_begin(config->log_byte);
    network_begin(config->network_ifaces, config->net_stats);
}

module_config_t* ddos2_modconfig(char* name, char* author, char* description,char* version){
     module_config_t* cfg=(module_config_t*)malloc(sizeof(module_config_t));
     cfg->arguments=array_create(1);
     cfg->name=(char*)malloc(sizeof(char)*strlen(name)+sizeof(char));
     cfg->author=(char*)malloc(sizeof(char)*strlen(author)+sizeof(char));
     cfg->description=(char*)malloc(sizeof(char)*strlen(description)+sizeof(char));
     cfg->version=(char*)malloc(sizeof(char)*strlen(version)+sizeof(char));
     strcpy(cfg->name,name);
     strcpy(cfg->author,author);
     strcpy(cfg->description,description);
     strcpy(cfg->version,version);

     return cfg;
}
