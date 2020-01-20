#!/bin/bash
gcc -c -fPIC *.c
gcc -shared -o ../../bin/modules/mod_a.so *.o 
