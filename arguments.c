//
//  arguments.c
//  ddos2
//
//  Created by Andre Zay on 16/01/2020.
//  Copyright © 2020 Andre Zay. All rights reserved.
//

#include "arguments.h"
#include "message.h"
#include "config.h"

#include <stdlib.h>
#include <string.h>

hashtable* arguments=NULL;
char* usage=NULL;

void arguments_begin(void){
    arguments=hashtbl_create(HASHTBL_CAPACITY);
}
