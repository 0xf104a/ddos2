#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ddos2/ddos2.h>
#include <ddos2/arguments.h>
#include <ddos2/message.h>
#include <ddos2/network.h>

#include "interface.h"

char* mod_name="mod_udp";
char* mod_author="Andrewerr@github.com";
char* mod_version="1.0a";
char* mod_description="Implements UDP interface.";

module_config_t* mod_on_load(program_config_t* config){
   ddos2_begin(config);
   debug("Initialized mod_udp.");
   /* Register interface */
   iface_t* iface=network_iface("udp");
   iface->connection_open=&udp_connection_open;
   iface->packet_send=&udp_packet_send;
   iface->connection_close=&udp_connection_close;
   iface->packet_receive=NULL;
   register_iface(iface);
   debug("Registered UDP interface");
   /* Add arguments */
   argument_add("--udp-timeout","Timeout for UDP interface", ARG_INT, argint(1000), true, false);
   argument_add("--udp-chunksize","Set chunksize for big UDP datagrams", ARG_INT,argint(1400),true,false);

   return ddos2_modconfig(mod_name,mod_author,mod_description,mod_version);
}

void mod_on_init(void){
    chunksize=argument_value_get("--udp-chunksize").intValue;
}

void mod_on_run(void){ //Stub
    warn("Module is not runnable.");
}

