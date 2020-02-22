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
   iface_t* iface=network_iface("udp");
   iface->connection_open=&udp_connection_open;
   iface->packet_send=&udp_packet_send;
   iface->connection_close=NULL;
   iface->packet_receive=NULL;
   register_iface(iface);
   debug("Registered interface");
   return ddos2_modconfig(mod_name,mod_author,mod_description,mod_version);
}

void mod_on_init(void){
   printf("mod_on_init() called.\n");
}

