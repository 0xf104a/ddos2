//
//  arguments.c
//  ddos2
//
//  Created by Andre Zay on 16/01/2020.
//  Copyright © 2020 Andre Zay. All rights reserved.
//

#include "arguments.h"
#include "message.h"
#include "hashtable.h"
#include "ddos2.h"

#include <stdlib.h>
#include <string.h>

hashtable* arguments=NULL;
void arguments_begin(hashtable* _arguments){
     arguments=_arguments;
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
    if(!hashtbl_check_key(arguments, argument->name)){
        hashtbl_add(arguments, argument->name, argument);
    }else{
        warn("Programming warning: Will not register argument `%s` – already registered(%s:%d)", argument->name,__FILE__, __LINE__);
    }
}

argument_t *argument_create(char* name, char* description, argtype type, bool compulsory, argvalue _default,bool has_default_value, bool is_help){
    argument_t* argument=(argument_t*)malloc(sizeof(argument_t));
    argument->name=(char*)malloc(sizeof(char)*(strlen(name)+1));
    argument->description=(char*)malloc(sizeof(char)*(strlen(description)+1));
    strcpy(argument->name, name);
    strcpy(argument->description, description);
    argument->type=type;
    argument->compulsory=compulsory;
    argument->value=_default;
    argument->is_set=has_default_value;
    argument->is_help=is_help;
    return argument;
}

void argument_add_compulsory(char* name, char* description, argtype type){
    if(_argcheck(name)){
        die("Programming error: rewriting argument: %s.(%s:%d)",name,__FILE__,__LINE__);
    }
    argvalue stub=argint(0);
    argument_t *argument=argument_create(name, description, type, true, stub, false, false);
    _register_argument(argument);
}

void argument_add(char* name, char* description, argtype type, argvalue _default, bool has_default_value, bool is_help){
    if(_argcheck(name)){
        die("Programming error: rewriting argument: %s.(%s:%d)",name,__FILE__,__LINE__);
    }
    argument_t *argument=argument_create(name, description, type, false, _default, has_default_value, is_help);
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

