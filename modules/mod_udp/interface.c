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

connection_t* connection_open(char* target){
    /* Params: target – stub: pass NULL */
    udp_descriptor_t* conn=(upd_connection_t*)malloc(sizeof(udp_descriptor_t));
    conn->fd=udp_socket();
    if(!conn->fd){
       free(conn);
       return NULL;
    }
    conn->time=time(NULL);
    /* Create interface structure */
    connection_t* connection=(connection_t*)malloc(sizeof(connection_t));
    connection->target=NULL;
    connection->is_open=true;
    connection->descriptor=conn;
    return connection;
}

bool packet_send(packet_t* packet){
    if(!packet->options){
       error("Programming error: %s: bad argrument: packet->options should not be NULL![%s:%d]",__FUNCTION__,__FILE__,__LINE__);
       return false;
     }
     if(!hashtbl_check_key(packet->options,"target")){
        error("Programming error: %s: bad argrument: packet passed has not enough option!Option 'target' is required.[%s:%d]",__FUNCTION__,__FILE__,__LINE__);
        return false;
     }
     char* target=(char*)hashtbl_get(packet->options,"target");
     if(!target){
        error("Programming error: %s: packet has bad option: target==NULL![%s:%d]",__FUNCTION__,__FILE__,__LINE__);
     }
     char* host=strtok(target,":");
     int port=atoi(strtok(NULL,":"));
     char* ip=hostname2ip(host);
     uint64_t chunksize=1200;//FIXME: set chunksize from options/arguments.
     return udp_sendto(((udp_descriptor_t*)packet->connection->descriptor)->fd, port, packet->payload, packet->sz, chunksize);
}


