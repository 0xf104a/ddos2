//
//  config.h
//  ddos2
//
//  Created by Andre Zay on 16/01/2020.
//  Copyright © 2020 Andre Zay. All rights reserved.
//

#ifndef config_h
#define config_h

#ifdef WIN32
#error Unsupported platform: Windows(WIN32)
#endif

#define HASHTBL_CAPACITY 256 //Default hashtable capacity
#define ARRAY_CPACITY    1   //Default array capacity
#define DEBUG            1   //Set 1 for debug mode
#define VERSION          "2.0a 2 573p5 fr0m h311" //Just version
#define MODULES_DIR      "modules/" //Directory where module's so files are stored
#define PATH_MAX_LEN     512 //Max length of path 

#if defined(__clang__)
#define __COMPILER "Clang"
#endif

#if defined(__GNUC__) && !defined(__COMPILER)
#define __COMPILER "GCC"
#endif

#ifndef __COMPILER
#warning "Unknown compiler: build may not be supported!"
#define __COMPILER "(Unknown)"
#endif

#endif /* config_h */
