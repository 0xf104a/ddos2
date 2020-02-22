#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ddos2/ddos2.h>
#include <ddos2/arguments.h>
#include <ddos2/hashtable.h>
#include <ddos2/message.h>
#include <ddos2/network.h>

#include "test.h"
#include "tests/udp.h"

char* mod_name="mod_a";
char* mod_author="Andrewerr@github.com";
char* mod_version="<none>";
char* mod_description="Contains basic tests for modules.";

module_config_t* mod_on_load(program_config_t* config){   
   ddos2_begin(config);
   argument_add("--test","Test program features(mod_a)",ARG_BOOL,argbool(false),true,false);
   udp_tests_prepare();
   return ddos2_modconfig(mod_name,mod_author,mod_description,mod_version);
}

void mod_on_init(void){
   if(!(argument_value_get("--test").boolValue)){
      return;
   }
   info("Performing tests");
   tests_begin();
   /*Register tests here*/
   udp_tests_register();
   /*Perform registered tests*/
   test_all();
}

