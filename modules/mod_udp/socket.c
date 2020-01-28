#include "socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include <ddos2/message.h>
#include <ddos2/network.h>

#include "util.h"
int udp_socket(void){
   int sock = socket(AF_INET, SOCK_DGRAM, 0);
   if(!sock){
      error("socket: %s(%d)[%s:%d]",strerror(errno),errno,__FILE__,__LINE__);
   }
   return sock;
}

int udp_sendto(char* target){
   struct sockaddr_in target;
   
      
