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

#include <stdlib.h>
#include <string.h>

hashtable* network_ifaces=NULL;

void network_begin(void){
    network_ifaces=hashtbl_create(HASHTBL_CAPACITY);
}

iface_t* network_iface(char* name){
    iface_t* iface=(iface_t*)malloc(sizeof(iface_t));
    iface->name=(char*)malloc(sizeof(char)*strlen(name)+sizeof(char));
    strcpy(iface->name, name);
    iface->bytes_sent=0;
    iface->packets_sent=0;
    return iface;
}

void register_iface(iface_t* iface){
    if(!network_ifaces){
        error("Programming error: network_ifaces==NULL! Have you called network_begin()?");
        return ;
    }
}
