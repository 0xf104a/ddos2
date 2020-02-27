#include "udp.h"
#include "../test.h"

#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <ddos2/message.h>
#include <ddos2/network.h>
#include <ddos2/arguments.h>

connection_t* conn;

bool test_init(void){
   iface_t* iface=get_iface("udp");
   if(!iface){
      return false;
    }
    return true;
}
bool test_open(void){
   iface_t* iface=get_iface("udp");
   conn=connection_open(iface,NULL);
   if(!conn){
       free(conn);
       return false;
   }
   return true;
}

bool test_send(void){
    iface_t* iface=get_iface("udp");
    char* packet_data=argument_value_get("--test-udp-data").strValue;
    char* packet_target=argument_value_get("--test-udp-target").strValue;
    packet_t* packet=packet_create(packet_target,packet_data,sizeof(char)*(strlen(packet_data)+1));
    packet->connection=conn;
    bool stat=iface->packet_send(packet);
    free(packet);
    return stat;
}
bool test_long_send(void){
    int64_t size=argument_value_get("--test-udp-long-packet-size").intValue;
    void* data=(void*)malloc(size*sizeof(char));
    char* packet_target=argument_value_get("--test-udp-target").strValue;
    packet_t* packet=packet_create(packet_target,data,size*sizeof(char));
    iface_t* iface=get_iface("udp");
    packet->connection=conn;
    bool stat =iface->packet_send(packet);
    free(packet);
    return stat;
}
bool test_close(void){
    iface_t* iface=get_iface("udp");
    return iface->connection_close(conn);
}
 
void udp_tests_prepare(void){
    argument_add("--test-udp-data","UDP test data which would be put in test packet.",ARG_STR,argstr("Hello,UDP!"),true,false);
    argument_add("--test-udp-target","UDP test target host",ARG_STR,argstr("127.0.0.1:1712"),true,false);
    argument_add("--test-udp-long-packet-size","UDP long packet size",ARG_INT,argint(8196),true,false);
}
 
void udp_tests_register(void){
   test_add("udp/init",&test_init);
   test_add("udp/open",&test_open);
   test_add("udp/send",&test_send);
   test_add("udp/long_send",&test_long_send);
    test_add("udp/close",&test_close);
}
