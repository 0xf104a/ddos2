#!/usr/bin/env bash

#  build.sh
#  ddos2
#
#  Created by Andre Zay on 16/01/2020.
#  Copyright © 2020 Andre Zay. All rights reserved.

#!/bin/bash
cd $(dirname `which $0`)

# Colors
if test -t 1; then
    bold=$(tput bold)
    normal=$(tput sgr0)
    red=$(tput setaf 1)
    green=$(tput setaf 2)
    blue=$(tput setaf 4)
    yellow=$(tput setaf 11)
else
    echo "[!]: No colors will be available: tput command was not found."
fi

echo " ______   ______   _______  _______  _______"
echo "(  __  \ (  __  \ (  ___  )(  ____ \/ ___   )"
echo "| (  \  )| (  \  )| (   ) || (    \/\/   )  |"
echo "| |   ) || |   ) || |   | || (_____     /   )"
echo "| |   | || |   | || |   | |(_____  )  _/   /"
echo "| |   ) || |   ) || |   | |      ) | /   _/"
echo "| (__/  )| (__/  )| (___) |/\\____) |(   (__/\\"
echo "(______/ (______/ (_______)\\_______)\_______/"
echo "                                     Build System v2.0-alpha"


error(){
    echo "${bold}${red}[-]:${normal}${1}"
}

success(){
    echo "${bold}${green}[+]:${normal}${1}"
}

warn(){
    echo "${bold}${yellow}[!]:${normal}${1}"
}

info(){
    echo "${bold}${blue}[*]:${normal}${1}"
}

exec(){
    info "${1}"
    eval "${1}"
    code=$?
    if [ ! $code -eq 0 ]; then
        error "Exec: ${1} failed with non-zero exit code: ${code}"
        exit -1
    fi
}

require_directory(){ # Creates directory if not exist
    if [ ! -d $1 ]; then
        warn "Directory not found: ${1}. Will create it now."
        exec "mkdir ${1}"
    fi
}

change_dir(){
    if [ ! -d $1 ]; then
        error "No such directory: ${1}"
        exit -1
    fi
    info "Entering directory: ${1}"
    cd $1
}

leave_dir(){
    info "Leaving directory: $(pwd)"
    cd $BASEDIR
}

if [ $# -eq 0 ]; then
    error "Please specify target. Use -h option for help."
    exit -1
fi

if [[ $1 == "-h" ]]; then
    echo "Usage:"$0" <-h> [all|debug|release]"
    echo "-h        Display this help message and exit."
    echo "debug     Build in debug mode"
    echo "release   Build in release mode"
    echo "clean     Remove obj/ bin/ directories."
    exit 0
fi

BASEDIR=`pwd`
CC="gcc"
CFLAGS="-c -I${BASEDIR} -Wall"
LD="ld"
LD_FLAGS="-ldl"
OBJ_DIR="obj/"
BIN_DIR="bin/"
MODULES_DIR="modules/"
MODULES_BIN="bin/modules/"
EXECUTABLE="ddos2"

declare -a SOURCES=("network" "commons" "array" "hashtable" "message" "module" "arguments" "main")
declare -a MODULES=("mod_a")

target_clean(){
   info "Cleaning up."
   exec "rm -rf ${OBJ_DIR}"
   exec "rm -rf ${BIN_DIR}"
   success "Cleaned."
}

target_debug(){
   CC="gcc-9"
   
   info "Building debug."
   require_directory $OBJ_DIR
   require_directory $BIN_DIR
   require_directory $MODULES_BIN
   leave_dir
   for file in "${SOURCES[@]}"
   do
       exec "${CC} ${CFLAGS} -fsanitize=address -fsanitize=undefined ${file}.c -o ${OBJ_DIR}${file}.o"
   done
   change_dir $OBJ_DIR
   objects=$(printf " %s.o" "${SOURCES[@]}")
   exec "${CC} -lasan -lubsan -o ${BASEDIR}/${BIN_DIR}${EXECUTABLE} ${objects}"
   leave_dir
}

target_release(){
   info "Building release."
   require_directory $OBJ_DIR
   require_directory $BIN_DIR
   require_directory $MODULES_BIN
   leave_dir
   for file in "${SOURCES[@]}"
   do
       exec "${CC} ${CFLAGS} -Ofast ${file}.c -o ${OBJ_DIR}${file}.o"
   done
   change_dir $OBJ_DIR
   objects=$(printf " %s.o" "${SOURCES[@]}")
   exec "${CC} ${LD_FLAGS} -o ${BASEDIR}/${BIN_DIR}${EXECUTABLE} ${objects}"
   leave_dir
}

target_modules(){
   info "Building modules."
   require_directory $MODULES_BIN
   for module in "${MODULES[@]}"
   do
     change_dir $MODULES_DIR
     change_dir $module
     exec ./build.sh all
     leave_dir
   done
}

target_all(){
   target_clean
   target_release
   target_modules
}
if [[ `type -t "target_${1}"` == "function" ]]; then
     eval "target_${1}"
     success "Done."
else
     error "No such target:${1}."
fi
