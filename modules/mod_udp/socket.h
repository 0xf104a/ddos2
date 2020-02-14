#ifndef SOCKET_H
#define SOCKET_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/time.h>

int udp_socket(void);
bool udp_sendto(int sock, char* _target,int port, void* payload,size_t size,     size_t chunksize);
bool udp_set_timeout(int sock,struct timeval tv);
#endif /* SOCKET_H */
