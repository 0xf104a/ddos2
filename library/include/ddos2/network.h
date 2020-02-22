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

#include <stdbool.h>
#include <stdint.h>

typedef struct _connection_t{
    char* target;
    bool is_open;
    void* descriptor;
    struct _iface_t* iface;
} connection_t;

typedef struct _packet_t{
    char* target;
    void* payload;
    struct _iface_t* iface;
    size_t sz;
    connection_t* connection; //if supported
    bool open_connection; //true if requires opening new connection
    hashtable* options;
} packet_t;

typedef struct _iface_t{
    char* name;
    uint64_t packets_sent;
    uint64_t bytes_sent;
    bool (*packet_send)(packet_t*);
    packet_t* (*packet_receive)(connection_t*);
    connection_t* (*connection_open)(char*);
    bool (*connection_close)(connection_t*);
    /*TODO:
    connection_t* connection_wait(iface_t*);
    packet_t* packet_listen(iface_t*);//Usefule with UDP
     */
} iface_t;

//extern hashtable* network_ifaces;
void network_begin(hashtable* ifaces, bool _stats);
void network_set_stats(bool stat);
iface_t* network_iface(char* name);
void register_iface(iface_t* iface);
connection_t* connection_open(iface_t* iface, char* target);
bool connection_close(connection_t* connection);
bool packet_send(iface_t* iface, packet_t* packet);
packet_t* packet_receive(connection_t* connection);
bool check_iface(char* name);
iface_t* get_iface(char* name);
array_t* list_ifaces(void);

#endif /* network_h */
