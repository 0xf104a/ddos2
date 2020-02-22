#include "udp.h"
#include "../test.h"

#include <stdbool.h>
#include <ddos2/message.h>
#include <ddos2/network.h>

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

void udp_tests_register(void){
   test_add("udp/init",&test_init);
   test_add("udp/open",&test_open);
}
