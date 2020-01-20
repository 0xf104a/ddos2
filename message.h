//
//  message.h
//  ddos-2
//
//  Created by Andre Zay on 08/07/2019.
//  Copyright © 2019 Andre Zay. All rights reserved.
//

#ifndef message_h
#define message_h

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#define LVL_DEBUG   0b0000001
#define LVL_INFO    0b0000010
#define LVL_SUCCESS 0b0000100
#define LVL_WARN    0b0001000
#define LVL_ERROR   0b0010000
#define LVL_FATAL   0b0100000
#define LVL_STATUS  0b1000000

#define LVL_FULL    0b1111111
#define LVL_RELEASE 0b1111110
#define LVL_NONE    0b0000000
#define LVL_FAST    0b0110000
#define LVL_STAT    0b1100000 //shows only status and fatal errors

#define HEADER "\033[95m"
#define OKBLUE "\033[94m"
#define OKGREEN "\033[92m"
#define ORANGE "\033[33m"
#define WARNING "\033[93m"
#define FAIL "\033[91m"
#define ENDC "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[33;5m"

#define INFO "%s[*]:%s", OKBLUE, ENDC
#define ERROR "%s[-]:%s", FAIL, ENDC
#define WARN "%s[!]:%s", WARNING, ENDC
#define SUCCESS "%s[+]:%s", OKGREEN, ENDC

#define PRINT_FORMATTED(STYLE) \
va_list args;              \
va_start(args, format);    \
flockfile(stdout);         \
printf("\33[2K");          \
printf(STYLE);             \
vprintf(format, args);     \
printf("\n");              \
funlockfile(stdout);       \
va_end(args)

#define PRINT_FORMATTED_NO_NEWLINE(STYLE) \
va_list args;                         \
va_start(args, format);               \
printf(STYLE);                        \
vprintf(format, args);                \
va_end(args)

extern uint8_t log_level;

void debug_warn(const char* format, ...);
void debug(const char* format, ...);
void info(const char* format, ...);
void warn(const char* format, ...);
void error(const char* format, ...);
void success(const char* format, ...);
void die(const char* format, ...);


void set_loglevel(uint8_t _log_level);
void add_loglevel(uint8_t _log_level);
void remove_loglevel(uint8_t _log_level);

#endif /* message_h */
