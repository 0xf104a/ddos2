#ifndef DOS_H
#define DOS_H

#include <stdint.h>

#include "network.h"

typedef enum{
	Byte,
	KiloBytes,
	MegaBytes,
	GigaBytes,
	TeraBytes
} speed_measure;

typedef struct{
	double speed;//Speed in bytes
	speed_measure measure;
	uint32_t refresh_time_ms;
	uint32_t duration;
} dos_status_t;

void dos_simple(packet_t* packet, double speed_limit);
/* 
 * Simple DoS attack in cycle
 * speed_limit -- max value for B/s(0.0 for no limit)
 * packet -- information about what and where should be send
 */

#endif
