#ifndef ddos2_h
#define ddos2_h

//#include "arguments.h"
//#include "module.h"
#include "hashtable.h"

typedef struct{
  hashtable* arguments;
  uint8_t* log_byte;
  char* version;
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
#endif
