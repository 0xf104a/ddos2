#ifndef SOCKET_H
#define SOCKET_H

int udp_socket(void);
bool udp_set_timeout(struct timeval tv);
bool udp_sendto(int sock, char* target,int port, void* payload,uint64_t size, uint32_t chunksize);

#endif /* SOCKET_H */
