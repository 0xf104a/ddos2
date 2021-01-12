//
//  commons.c
//  ddos2
//
//  Created by Andre Zay on 19/01/2020.
//  Copyright © 2020 Andre Zay. All rights reserved.
//

#include "commons.h"
#include "config.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


bool is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void ch_local_dir(const char* argv0){
    /*
     Sets wroking direcotry to executable's directory
     */
    char* work_dir=(char*)malloc(sizeof(char)*(strlen(argv0)+1));
    strcpy(work_dir,argv0);
    uint64_t i=strlen(work_dir)-1;
    for(;i>=0;i--){
        if(work_dir[i]=='/'){
            work_dir[i]='\0';
            break;
        }
    }
    chdir(work_dir);
    free(work_dir);
}
