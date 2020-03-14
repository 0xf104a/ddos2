#include "test.h"

#include <ddos2/message.h>
#include <ddos2/array.h>

array_t* tests=NULL;

void tests_begin(void){
    tests=array_create(1);
}

void test_add(const char* name, bool (*test_fun)(void)){
    if(!tests){
       die("Programming error: tests was not initialized[%s:%d]",__FILE__,__LINE__);
    }
    test_t* test=(test_t*)malloc(sizeof(test_t));
    test->name=name;
    test->perform=test_fun;
    array_add(tests,test);
}

void test_all(void){
    if(!tests){
       die("Programming error: tests was not initialized[%s:%d]",__FILE__,__LINE__);
    }
    int i=0;
    info("%d tests to perform",tests->sz);
    for(;i<tests->sz;++i){
        test_t* test=(test_t*)tests->base[i];
        printf("Testing %s...",test->name);
        fflush(stdout);
        if(test->perform()){
           printf("%s%sOK%s\n",BOLD,OKGREEN,ENDC);
        }else{
           printf("%s%sFAIL%s\n",BOLD,FAIL,ENDC);
           die("Some tests has failed!");
	}
    }
    success("All tests passed.");
}
    
