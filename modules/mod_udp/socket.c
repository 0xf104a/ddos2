#include "socket.h"

#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <time.h>
#include <string.h>

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

bool udp_set_timeout(int sock,struct timeval tv){
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
            error("setsocopt: %s(%d)[%s:%d]",__FILE__,__LINE__);
            return false;
    }
    return true;
}
     
bool udp_sendto(int sock, char* _target,int port, void* payload,size_t size, size_t chunksize){
   char* host=hostname2ip(_target);
   if(!host){
      return false;
   }
   struct sockaddr_in target;
   target.sin_family = AF_INET;
   target.sin_port = htons(port);
   target.sin_addr.s_addr = inet_addr(host);
   
   while(size>0){
      if(sendto(sock, payload, u32_min(chunksize, size),  0, (struct sockaddr*)&target, sizeof(target)) == -1){
          error("UDP sendto %s:%d failed: %s(%d)", host, port, strerror(errno),errno);
          return false;
      }
      payload+=u32_min(chunksize, size);
      size-=u32_min(chunksize, size);
   }
   
   return true;
}


