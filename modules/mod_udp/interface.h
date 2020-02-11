#ifndef INTERFACE_H
#define INTERFACE_H

#include <ddos2/network.h>
#include <time.h>

typedef struct{
    int fd;
    time_t open_time;
} udp_descriptor_t;


#endif
