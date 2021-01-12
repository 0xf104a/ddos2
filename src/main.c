//
//  main.c
//  ddos2
//
//  Created by Andre Zay on 16/01/2020.
//  Copyright © 2020 Andre Zay. All rights reserved.
//

#include <stdio.h>
#include "message.h"
#include "arguments.h"
#include "commons.h"
#include "module.h"
#include "network.h"
#include "config.h"
#include "status.h"

int main(int argc, const char * argv[]) {
    printf(BLINK);
    printf(WARNING);
    printf(BOLD);
    printf(" (    (        )  (\n");
    printf(" )\\ ) )\\ )  ( /(  )\\ )      )\n");
    printf("(()/((()/(  )\\())(()/(   ( /(\n");
    printf("/(_))/(_))((_)\\  /(_))  )(_))\n");
    printf("(_))_(_))_   ((_)(_))   ((_)\n");
    printf(ENDC);
    printf(WARNING);
    printf(BOLD);
    printf("|   \\|   \\ / _ \\/ __|  |_  )\n");
    printf("| |) | |) | (_) \\__ \\   / /\n");
    printf("|___/|___/ \\___/|___/  /___| \n");
    printf("                       Version %s\n",VERSION);
    printf(ENDC);
#if DEBUG
    set_loglevel(LVL_FULL);
#else
    set_loglevel(LVL_RELEASE);
#endif
    
    info("Built with %s %s at %s %s",__COMPILER,__VERSION__,__DATE__,__TIME__);
    ch_local_dir(argv[0]);
    
    arguments_begin();
    modules_begin();
    network_begin();
    modules_configure(VERSION);
    modules_load(MODULES_DIR);
    
    argument_add_compulsory("--module", "Module to run.", ARG_STR);
    argument_add("--ls-modules","List all modules loaded.",ARG_BOOL,argbool(false),true,true);
    argument_add("--mod-summary","Show extended information about module.",ARG_BOOL,argbool(false),true,false);
    argument_add("--ls-ifaces", "List network interfaces.", ARG_BOOL,argbool(false),true,true);//DONE: This is, so called help argument. Should set that after implementing it in arguments.c
    argument_add("--net-no-stats", "Disable packets and byte counting for interfaces", ARG_BOOL, argbool(false),true,false);
    argument_add("--hide-stats", "Do not show statistics of DoS when attack is in progress(may improve preformance)", ARG_BOOL, argbool(false), true, false);   
    argument_add("--target", "Target to attack", ARG_STR, argstr((char*)NULL), false, false);  

    arguments_parse(argc, argv, 1);
    
    
    if(argument_value_get_s("--ls-modules", ARG_BOOL).boolValue){
        modules_list();
        return 0;
    }

    if(argument_value_get_s("--ls-ifaces", ARG_BOOL).boolValue){
        network_print_ifaces();
        return 0;
    }

    if(argument_value_get_s("--mod-summary", ARG_BOOL).boolValue){
        argvalue value=argument_value_get_s("--module", ARG_STR);
        char* mod_name=value.strValue;
        module_t* module=module_get(mod_name);
        module_summary(module);
        return 0;
    }
    
    if(!argument_value_get_s("--hide-stats", ARG_BOOL).boolValue){
        status_begin(argument_value_get_s("--target",ARG_STR).strValue);
    }
    modules_on_init();
    
    module_t* mod=module_get(argument_value_get_s("--module", ARG_STR).strValue);
    mod->mod_on_run(); //Run selected module
    
    return 0;
}
