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
    argument_add("--help", "Show help message.", ARG_BOOL, argbool(false), false);
}

bool _argcheck(char *name){
    if(!arguments){
        die("Programming error: arguments==NULL!Have you called arguments_begin()?(%s:%d)");
    }
    return hashtbl_check_key(arguments, name);
}

void _register_argument(argument_t *argument){
    if(!arguments){
        die("Programming error: arguments==NULL!Have you called arguments_begin()?(%s:%d)");
    }
    hashtbl_add(arguments, argument->name, argument);
}


argument_t *argument_create(char* name, char* description, argtype type, bool compulsory, argvalue _default,bool has_default_value){
    argument_t* argument=(argument_t*)malloc(sizeof(argument_t));
    argument->name=(char*)malloc(sizeof(char)*(strlen(name)+1));
    argument->description=(char*)malloc(sizeof(char)*(strlen(description)+1));
    strcpy(argument->name, name);
    strcpy(argument->description, description);
    argument->type=type;
    argument->compulsory=compulsory;
    argument->value=_default;
    argument->is_set=has_default_value;
    return argument;
}

void argument_add_compulsory(char* name, char* description, argtype type){
    if(_argcheck(name)){
        die("Programming error: rewriting argument: %s.(%s:%d)",name,__FILE__,__LINE__);
    }
    argvalue stub=argint(0);
    argument_t *argument=argument_create(name, description, type, true, stub, false);
    _register_argument(argument);
}

void argument_add(char* name, char* description, argtype type, argvalue _default, bool has_default_value){
    if(_argcheck(name)){
        die("Programming error: rewriting argument: %s.(%s:%d)",name,__FILE__,__LINE__);
    }
    argument_t *argument=argument_create(name, description, type, false, _default, has_default_value);
    _register_argument(argument);
}

argument_t* argument_get(char *name){
    if(!_argcheck(name)){
        die("Programming error: Requested unknown argument: %s.(%s:%d).",name,__FILE__,__LINE__);
    }
    return (argument_t*)hashtbl_get(arguments, name);
}

bool argument_check(char *name){
    argument_t *argument=argument_get(name);
    return argument->is_set;
}

argvalue argument_value_get(char *name){
    argument_t *argument=argument_get(name);
    return argument->value;
}
void arguments_parse(int argc, const char * argv[], int start){
    if(!arguments){
        die("Programming error: arguments==NULL!Have you called arguments_begin()?(%s:%d)");
    }
    int i=start;
    //Parse
    while (i<argc){
        char *argname=(char*)malloc(sizeof(char)*strlen(argv[i])+sizeof(char));
        strcpy(argname, argv[i]);
        if(!hashtbl_check_key(arguments, argname)){
            die("Unknown argument: %s.",argname);
        }
        argument_t *argument=argument_get(argname);
        if(argument->type==ARG_BOOL){
            argument->value=argbool(true);
            argument->is_set=true;
        }
        if(argument->type==ARG_INT){
            ++i;
            if(i>=argc){
                die("Argument %s require a value.",argument->name);
            }
            argument->value=argint(atoll(argv[i]));
            argument->is_set=true;
        }
        if(argument->type==ARG_STR){
            ++i;
            if(i>=argc){
                die("Argument %s require a value.",argument->name);
            }
            char* argvalue=(char*)malloc(sizeof(char)*strlen(argv[i])+sizeof(char));
            strcpy(argvalue, argv[i]);
            argument->value.strValue=argvalue;
            argument->is_set=true;
        }
        free(argname);
        ++i;
    }
    //Check if need to show help
    if(argument_value_get("--help").boolValue){
        arguments_help(argv[0]);
        exit(0);
    }
    //Check that all compulsory arguments are set
    for(i=0;i<arguments->values->sz;++i){
        argument_t *argument = (argument_t*)arguments->values->base[i];
        if((!argument->is_set)&&argument->compulsory){
            die("Argument %s is required.",argument->name);
        }
    }
}

void arguments_help(const char *progname){
    if(!usage){
        warn("Programming warning:usage is unset(%s:%d)", __FILE__, __LINE__);
    }
    int i=0;
    info("Usage: %s %s",progname, usage);
    for(;i<arguments->values->sz;++i){
        argument_t *argument = (argument_t*)arguments->values->base[i];
        printf("    %s – %s\n",argument->name, argument->description);
    }
}
