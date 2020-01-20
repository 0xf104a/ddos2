//
//  network.h
//  ddos2
//
//  Created by Andre Zay on 20/01/2020.
//  Copyright © 2020 Andre Zay. All rights reserved.
//

#ifndef network_h
#define network_h

#include "hashtable.h"
#include "array.h"
#include "config.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct{
    char* target;
    bool is_open;
    void* descriptor;
} connection_t;

typedef struct{
    char* target;
    char* payload;
    char* iface;
    size_t sz;
    connection_t* connection; //if supported
    bool open_connection; //true if requires opening new connection
} packet_t;

typedef struct{
    char* name;
    uint64_t packets_sent;
    uint64_t bytes_sent;
    void (*packet_send)(packet_t*);
    packet_t* (*packet_receive)(connection_t*);
    connection_t* (*connection_open)(char*);
    void (*connection_close)(connection_t*);
} iface_t;

extern hashtable* network_ifaces;

#endif /* network_h */
