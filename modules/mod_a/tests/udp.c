#include "udp.h"
#include "../test.h"

#include <stdbool.h>
#include <string.h>

#include <ddos2/message.h>
#include <ddos2/network.h>
#include <ddos2/arguments.h>

bool test_init(void){
   iface_t* iface=get_iface("udp");
   if(!iface){
      return false;
    }
    return true;
}
bool test_open(void){
   iface_t* iface=get_iface("udp");
   connection_t* conn=connection_open(iface,NULL);
   if(!conn){
      free(conn);
      return false;
   }
   free(conn);
   return true;
}

bool test_send(void){
    iface_t* iface=get_iface("udp");
    connection_t* conn=connection_open(iface,NULL);
    char* packet_data=argument_value_get("--test-udp-data").strValue;
    char* packet_target=argument_value_get("--test-udp-target").strValue;
    packet_t* packet=packet_create(packet_target,packet_data,sizeof(char)*(strlen(packet_data)+1));
    packet->connection=conn;
    return iface->packet_send(packet);
}
void udp_tests_prepare(void){
    argument_add("--test-udp-data","UDP test data which would be put in test packet.",ARG_STR,argstr("Hello,UDP!"),true,false);
    argument_add("--test-udp-target","UDP test target host",ARG_STR,argstr("127.0.0.1:1712"),true,false);

}
 
void udp_tests_register(void){
   test_add("udp/init",&test_init);
   test_add("udp/open",&test_open);
   test_add("udp/send",&test_send);
}
