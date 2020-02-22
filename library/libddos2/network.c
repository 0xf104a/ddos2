//
//  network.c
//  ddos2
//
//  Created by Andre Zay on 20/01/2020.
//  Copyright © 2020 Andre Zay. All rights reserved.
//

#include "network.h"
#include "arguments.h"
#include "message.h"
#include "hashtable.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

hashtable* network_ifaces=NULL;
bool stats=true;

void network_begin(hashtable* ifaces, bool _stats){
    network_ifaces=ifaces;
    stats=_stats;
}

void network_set_stats(bool stat){//Sets wheteher we need to collect interface statisitcs
    stats=stat;
}

iface_t* network_iface(char* name){
    iface_t* iface=(iface_t*)malloc(sizeof(iface_t));
    iface->name=(char*)malloc(sizeof(char)*strlen(name)+sizeof(char));
    strcpy(iface->name, name);
    iface->bytes_sent=0;
    iface->packets_sent=0;
    iface->connection_close=NULL;
    iface->connection_open=NULL;
    iface->packet_send=NULL;
    iface->packet_receive=NULL;
    return iface;
}
void register_iface(iface_t* iface){
    if(!network_ifaces){
        error("Programming error: network_ifaces==NULL! Have you called network_begin()?");
        return ;
    }
    hashtbl_add(network_ifaces, iface->name, iface);
    debug("Registered network interface: %s",iface->name);
}

connection_t* connection_open(iface_t* iface, char* target){
    if(!iface){
        error("Programming error: %s: Bad argument: iface(%s:%d)",__FUNCTION__,__FILE__,__LINE__);
        return NULL;
    }
    if(!iface->connection_open){
        error("Programming error: Interface %s does not support openning connections.",iface->name);
        return NULL;
    }
    connection_t* connection=iface->connection_open(target);
    if(!connection){
       debug_warn("NULL as connection was returned!");
       return NULL;
     } 
    connection->iface=iface;
    return connection;
}

bool connection_close(connection_t* connection){
    if(!connection){
        error("Programming error: %s: Bad argument: connection(%s:%d)",__FUNCTION__,__FILE__,__LINE__);
        return false;
    }
    bool result = connection->iface->connection_close(connection);
    free(connection);
    return result;
}

bool _packet_send(packet_t* packet){
    if(!packet){
        error("Programming error: %s: Bad argument: packet is NULL(%s:%d)",__FUNCTION__,__FILE__,__LINE__);
        return false;
    }
    if(!packet->iface){
        error("Programming error: %s: Bad argument: packet->iface is NULL(%s:%d)",__FUNCTION__,__FILE__,__LINE__);
        return false;
    }
    if(!packet->iface->packet_send){
        error("Programming error: %s: Not supported: Interface %s does not support packet_send(%s:%d)",__FUNCTION__,__FILE__,__LINE__);
        return false;
    }
    if(stats){
        packet->iface->bytes_sent+=packet->sz;
        packet->iface->packets_sent++;
    }
    return packet->iface->packet_send(packet);
}

bool packet_send(iface_t* iface, packet_t* packet){
    if(!packet){
        error("Programming error: %s: Bad argument: packet is NULL(%s:%d)",__FUNCTION__,__FILE__,__LINE__);
        return false;
    }
    packet->iface=iface;
    return _packet_send(packet);
}

packet_t* packet_receive(connection_t* connection){
    if(!connection){
        error("Programming error: %s: Bad argument: connection is NULL(%s:%d)",__FUNCTION__,__FILE__,__LINE__);
        return NULL;
    }
    if(!connection->iface){
        error("Programming error: %s: Bad argument: connection is NULL(%s:%d)",__FUNCTION__,__FILE__,__LINE__);
        return NULL;
    }
    if(!connection->iface->packet_receive){
        error("Programming error: Interface %s does not support openning connections.",connection->iface->name);
        return NULL;
    }
    
    return connection->iface->packet_receive(connection);
}

packet_t* packet_create(char* target, void* payload,size_t sz){
       packet_t* packet=(packet_t*)malloc(sizeof(packet_t));
       packet->target=target;//No copying for optimization
       packet->payload=payload;
       packet->sz=sz;
       packet->options=hashtbl_create(1);//Also for optimization
       packet->open_connection=false;
       packet->connection=NULL;
       return packet;
}

bool check_iface(char* name){ //Checks interface is available
    return hashtbl_check_key(network_ifaces,name);
}

iface_t* get_iface(char* name){ //Returns interface by name 
    return (iface_t*)hashtbl_get(network_ifaces,name);
}

array_t* list_ifaces(void){
    return network_ifaces->values;
}


