#ifndef STATUS_H
#define STATUS_H

#include <stddef.h>
#include <time.h>

typedef enum{
     byte,
     kilobyte,
     megabyte,
     gigabyte,
     terrabyte
} sz_measure;

typedef struct{
     double speed; //Bytes per seconds
     char* target;
     double total_sent;
     double delta;
     clock_t last_updated; 
} status_t;

void status_begin(char* target);

#endif
