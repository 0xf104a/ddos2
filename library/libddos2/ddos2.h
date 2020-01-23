#ifndef ddos2_h
#define ddos2_h

//#include "arguments.h"
//#include "module.h"
#include "hashtable.h"

typedef struct{
    hashtable* arguments;
    hashtable* network_ifaces;
    bool net_stats;
    const char* version;
    uint8_t* log_byte;
} program_config_t;

typedef struct{
    array_t* arguments;
    char* name;
    char* author;
    char* description;
    char* version;
} module_config_t;

//typedef struct _program_config_t program_config_t;
//typedef struct _module_config_t module_config_t;
void ddos2_begin(program_config_t* config);
module_config_t* ddos2_modconfig(char* name, char* author, char* description    ,char* version);
#endif
