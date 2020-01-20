//
//  commons.c
//  ddos2
//
//  Created by Andre Zay on 19/01/2020.
//  Copyright © 2020 Andre Zay. All rights reserved.
//

#include "commons.h"
#include "config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


bool is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void ch_local_dir(void){
    /*
     Sets wroking direcotry to executable's directory
     */
    char* work_dir;
    
}
