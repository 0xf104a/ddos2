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
#include "config.h"

int main(int argc, const char * argv[]) {
    printf(ORANGE);
    printf(BOLD);
    printf(BLINK);
    printf(" (    (        )  (\n");
    printf(" )\\ ) )\\ )  ( /(  )\\ )      )\n");
    printf("(()/((()/(  )\\())(()/(   ( /(\n");
    printf("/(_))/(_))((_)\\  /(_))  )(_))\n");
    printf("(_))_(_))_   ((_)(_))   ((_)\n");
    printf(ENDC);
    printf(ORANGE);
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
    arguments_begin();
    modules_begin();
    modules_configure(VERSION);
    modules_load(MODULES_DIR);
    /**Arguments**/
    argument_add_compulsory("--module", "Module to run.", ARG_STR);
    argument_add("--ls-modules","List all modules loaded.",ARG_BOOL,argbool(false),true);
    /**Parse**/
    arguments_parse(argc, argv, 1);
    
    return 0;
}
