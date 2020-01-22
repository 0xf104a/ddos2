#include "ddos2.h"
#include "message.h"
#include "arguments.h"

void ddos2_begin(program_config_t* config){
    arguments_begin(config->arguments);
    message_begin(config->log_byte);
}
