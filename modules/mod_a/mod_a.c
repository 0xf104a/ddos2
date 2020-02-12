#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ddos2/ddos2.h>
#include <ddos2/arguments.h>
#include <ddos2/hashtable.h>
#include <ddos2/message.h>
char* mod_name="mod_a";
char* mod_author="Andrewerr@github.com";
char* mod_version="<none>";
char* mod_description="Useless module for debugging.";
module_config_t* mod_config_pull(program_config_t* config){
   
   ddos2_begin(config);
   argument_add("--mod-a","Super useless one.",ARG_BOOL,argbool(false),true,false);
   info("H3110, w0r1d!");
   return ddos2_modconfig(mod_name,mod_author,mod_description,mod_version);
}
void mod_on_init(void){
   printf("mod_on_init() called.\n");
}

