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
#include "config.h"

int main(int argc, const char * argv[]) {
#if DEBUG
    set_loglevel(LVL_FULL);
#else
    set_loglevel(LVL_RELEASE);
#endif
    arguments_begin();
    /**Arguments**/
    
    /**Parse**/
    arguments_parse(argc, argv, 1);
    
    return 0;
}
