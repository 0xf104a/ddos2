#include "interface.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ddos2/network.h>
#include <ddos2/message.h>

#include "socket.h"
#include "util.h"
/*
   NOTE: There is no connections in UDP proto, but still any socket opened 
         is needed to be stored. Parameter target would be ignored. Real 
         packet target should be specified in it's options.
*/

connection_t* udp_connection_open(char* target){
    /* Params: target – stub: pass NULL */
    udp_descriptor_t* conn=(udp_descriptor_t*)malloc(sizeof(udp_descriptor_t));
    conn->fd=udp_socket();
    if(!conn->fd){
       free(conn);
       return NULL;
    }
    conn->open_time=time(NULL);
    /* Create interface structure */
    connection_t* connection=(connection_t*)malloc(sizeof(connection_t));
    connection->target=NULL;
    connection->is_open=true;
    connection->descriptor=conn;
    return connection;
}

bool udp_packet_send(packet_t* packet){
    if(!packet->options){
       error("Programming error: %s: bad argrument: packet->options should not be NULL![%s:%d]",__FUNCTION__,__FILE__,__LINE__);
       return false;
     }
     if(!packet->target){
        error("Programming error: %s: packet has bad option: target==NULL![%s:%d]",__FUNCTION__,__FILE__,__LINE__);
     }
     char* target=(char*)malloc(sizeof(char)*(strlen(packet->target)+1));
     strcpy(target,packet->target);
     char* host=strtok(target,":");
     int port=atoi(strtok(NULL,":"));
     char* ip=hostname2ip(host);
     uint64_t chunksize=1200;//FIXME: set chunksize from options/arguments.
     bool stat=udp_sendto(((udp_descriptor_t*)packet->connection->descriptor)->fd,host, port, packet->payload, packet->sz, chunksize);
     free(ip);
     free(target);
     return stat;
}


bool udp_connection_close(connection_t* connection){
    bool stat=udp_close(((udp_descriptor_t*)connection->descriptor)->fd);
    free(connection->descriptor);
    free(connection);
    return stat;
}

