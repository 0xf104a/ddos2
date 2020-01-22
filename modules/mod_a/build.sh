#!/bin/bash
gcc-9 -c -fPIC mod_a.c -I../../library/include/
gcc-9 -shared -o ../../bin/modules/mod_a.so *.o -L../../lib/ -lddos2
