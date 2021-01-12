#include "status.h"
#include "message.h"

#include <time.h>
#include <stdlib.h>

status_t* _status = NULL;

void status_begin(char* target){
    _status = (status_t*)malloc(sizeof(status_t));
    _status->speed=0.0;
    _status->total_sent=0.0;
    _status->last_updated=clock();
    _status->target=target;
    _status->delta=0.0;
}

void status_update(uint64_t delta_bytes){
    _status->total_sent+=delta_bytes;
    _status->delta+=delta_bytes;
}

void status_print(void){
    clock_t now=clock();
    double delta_t=(now-_status->last_updated)/CLOCKS_PER_SEC;
    _status->speed=_status->delta/delta_t;
    fflush(stdout);
    status("Attacking target %s, speed %.2f, total sent: %.2f",_status->target,_status->speed,_status->total_sent);
}
