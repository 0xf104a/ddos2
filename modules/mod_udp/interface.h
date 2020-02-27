#ifndef INTERFACE_H
#define INTERFACE_H

#include <ddos2/network.h>
#include <time.h>

typedef struct{
    int fd;
    time_t open_time;
} udp_descriptor_t;

connection_t* udp_connection_open(char* target);
bool udp_packet_send(packet_t* packet);
bool udp_connection_close(connection_t* connection);

#endif
