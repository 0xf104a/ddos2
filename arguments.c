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
    argument_add("--help", "Show help message.", ARG_BOOL, argbool(false), false, true);
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


argument_t *argument_create(char* name, char* description, argtype type, bool compulsory, argvalue _default,bool has_default_value, bool is_help, bool array){
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
    argument->values=NULL;
    if(array){
        argument->values=array_create(1);
    }
    return argument;
}

void argument_add_compulsory(char* name, char* description, argtype type){
    /* Adds compulsory argument
     name – Name of an argument
     description – description of an argument
     type – type of an argument.
     */
    if(_argcheck(name)){
        die("Programming error: rewriting argument: %s.(%s:%d)",name,__FILE__,__LINE__);
    }
    argvalue stub=argint(0);
    argument_t *argument=argument_create(name, description, type, true, stub, false, false, false);
    _register_argument(argument);
}

void argument_add(char* name, char* description, argtype type, argvalue _default, bool has_default_value, bool is_help){
    /*
        name – Name of an argument
        description – description of an argument
        type – type of an argument.
        _default – default value
        has_default_value – set true if _default is not stub
        is_help – whether argument is used to show help information
     */
    if(_argcheck(name)){
        die("Programming error: rewriting argument: %s.(%s:%d)",name,__FILE__,__LINE__);
    }
    argument_t *argument=argument_create(name, description, type, false, _default, has_default_value, is_help, false);
    _register_argument(argument);
}

void argument_add_array(char* name, char* description,argtype type, bool compulsory){
    /* Adds argument that creates array of argvalue's
       name – Name of an argument
       description – description of an argument
       type – type of an argument. ARG_BOOL obviously not supported.
       compulosry – whether argument needs at least 1 value
     */
    argument_t* argument=argument_create(name, description, type, compulsory, argbool(false), false, false, true);
    _register_argument(argument);
}

argument_t* argument_get(char *name){
    /* Get argument instance
       name – name of arguments*/
    if(!_argcheck(name)){
        die("Programming error: Requested unknown argument: %s.(%s:%d).",name,__FILE__,__LINE__);
    }
    return (argument_t*)hashtbl_get(arguments, name);
}

bool argument_check(char *name){
    /* checks whether argument was set*/
    argument_t *argument=argument_get(name);
    return argument->is_set;
}

argvalue argument_value_get(char *name){
    /* get argument value */
    argument_t *argument=argument_get(name);
    return argument->value;
}
void arguments_parse(int argc, const char * argv[], int start){
    /* Pasrses arguments
       argc – arguments count
       argv – arguments values
       start – number of argument from which to start parseing*/
    if(!arguments){
        die("Programming error: arguments==NULL!Have you called arguments_begin()?(%s:%d)");
    }
    int i=start;
    bool help_flag=false;//This set iff some help argument is set
    //Parse
    while (i<argc){
        char *argname=(char*)malloc(sizeof(char)*strlen(argv[i])+sizeof(char));
        strcpy(argname, argv[i]);
        if(!hashtbl_check_key(arguments, argname)){
            die("Unknown argument: %s.",argname);
        }
        argument_t *argument=argument_get(argname);
        if(argument->is_help){
            help_flag=true;
        }
        if(argument->type==ARG_BOOL){
            argument->value=argbool(true);
            if(argument->values){
                die("Programming error:%s:Bad argument: bool arrays are not supported(%s:%d)", __FUNCTION__,__FILE__,__LINE__);
            }
            argument->is_set=true;
        }
        if(argument->type==ARG_INT){
            ++i;
            if(i>=argc){
                die("Argument %s requires a value.",argument->name);
            }
            argument->value=argint(atoll(argv[i]));
            if(argument->values){
                array_add(argument->values, &argument->value);
            }
            argument->is_set=true;
        }
        if(argument->type==ARG_STR){
            ++i;
            if(i>=argc){
                die("Argument %s requires a value.",argument->name);
            }
            char* argvalue=(char*)malloc(sizeof(char)*strlen(argv[i])+sizeof(char));
            strcpy(argvalue, argv[i]);
            argument->value.strValue=argvalue;
            if(argument->values){
                array_add(argument->values, &argument->value);
            }
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
    if(help_flag){
        return ;//We don't need to check anything – there is help arguments
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
    /* Shows help for an prgoram
       progname – name of program(ususally argv[0])*/
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
argvalue argument_value_get_s(char *name, argtype type){//Secure version of argument_value_get
    /* The same as argument_value_get but with type check.
       Useful for modules. */
    argument_t *argument=argument_get(name);
    if(argument->type!=type){
        die("Programming error: argument %s has wrong type!(%s:%d)",name,__FILE__,__LINE__);
    }
    return argument->value;
}
