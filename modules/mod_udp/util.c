#include "util.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ddos2/message.h>

char* hostname2ip(const char* hostname)
{
    char* ip = (char*)malloc(16*sizeof(char));//255.255.255.255\0
    struct hostent* he;
    struct in_addr** addr_list;
    int i;

    if ((he = gethostbyname(hostname)) == NULL) {
        // get the host info
        error("Failed to find host %s", hostname);
        return NULL;
    }

    addr_list = (struct in_addr**)he->h_addr_list;

    for (i = 0; addr_list[i] != NULL; i++) {
        //Return the first one;
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return ip;
    }
    return NULL;
}

uint32_t u32_min(uint32_t a, uint32_t b){
    if(a<b){
       return a;
    }
    return b;
}
