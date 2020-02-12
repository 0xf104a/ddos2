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
    echo "[!]: No colors will be available: not supported."
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

check_equal(){
    printf "${bold}${blue}[*]:${normal}Checking equality: ${1} and ${2}..."
    if ! cmp $1 $2 >/dev/null 2>&1
    then
      printf "${bold}${red}FAILED${normal}\n"
      return -1
    fi
    printf "${bold}${green}OK${normal}\n"
    return 0
}

require_equal(){
    check_equal $1 $2
    local r=$?
    if [ $r -eq -1 ]; then
       error "Files ${1} and ${2} are not equal."
       exit -1
    fi
}

check_command(){
    printf "${bold}${blue}[*]:${normal}Checking that ${1} avail..."
    if ! [ -x "$(command -v ${1})" ]; then
      printf "${bold}${red}FAILED${normal}\n"
      return -1
    fi
    printf "${bold}${green}OK${normal}\n"
    return 0
}

require_command(){
    check_command $1
    local r=$?
    if [ $r -eq -1 ]; then
       error "Command ${1} is not avail."
       exit -1
    fi
}

if [ $# -eq 0 ]; then
    error "Please specify target. Use -h option for help."
    exit -1
fi

if [[ $1 == "-h" ]]; then
    echo "Usage:"$0" <-h> [all|debug|release|clean|library]"
    echo "-h        Display this help message and exit."
    echo "debug     Build in debug mode"
    echo "release   Build in release mode"
    echo "library   Build libddos2"
    echo "all       Build all in release mode"
    echo "all-debug Build all in debug mode"
    echo "clean     Remove obj/ bin/ directories."
    exit 0
fi

BASEDIR=`pwd`
CC="gcc"
CFLAGS="-Wall -I${BASEDIR}/library/libddos2"
LD="ld"
LD_FLAGS="-ldl"
OBJ_DIR="obj/"
BIN_DIR="bin/"
LIB_DIR="lib/"
MODULES_DIR="modules/"
MODULES_BIN="bin/modules/"
EXECUTABLE="ddos2"

declare -a SOURCES=("message" "array" "hashtable" "commons" "network" "module" "arguments" "main")
declare -a MODULES=("mod_a")

target_check(){
   require_command $CC
   require_command $LD
}
target_clean(){
   info "Cleaning up."
   exec "rm -rf ${OBJ_DIR}"
   exec "rm -rf ${BIN_DIR}"
   exec "rm -rf ${LIB_DIR}"
   success "Cleaned."
}

target_library(){
   change_dir "library/libddos2"
   exec "./build.sh release" #TODO:In debug – set debug target
   leave_dir
}

target_library-debug(){
   change_dir "library/libddos2"
   exec "./build.sh debug"
   leave_dir
}

target_debug(){
   target_check
   CC="gcc-9"
   require_command $CC
   info "Building debug."
   require_directory $OBJ_DIR
   require_directory $BIN_DIR
   require_directory $MODULES_BIN
   leave_dir
   for file in "${SOURCES[@]}"
   do
       exec "${CC} -c ${CFLAGS} -fsanitize=address -fsanitize=undefined ${file}.c -o ${OBJ_DIR}${file}.o"
   done
   change_dir $OBJ_DIR
   objects=$(printf " %s.o" "${SOURCES[@]}")
   exec "${CC} ${LD_FLAGS} -lasan -lubsan -o ${BASEDIR}/${BIN_DIR}${EXECUTABLE} ${objects}"
   leave_dir
   success "Succesfully built debug."
}

target_release(){
   info "Building release."
   require_directory $OBJ_DIR
   require_directory $BIN_DIR
   require_directory $MODULES_BIN
   for file in "${SOURCES[@]}"
   do
       exec "${CC} -c ${CFLAGS} -Ofast ${file}.c -o ${OBJ_DIR}${file}.o"
   done
   change_dir $OBJ_DIR
   objects=$(printf " %s.o" "${SOURCES[@]}")
   exec "${CC} ${LD_FLAGS} -o ${BASEDIR}/${BIN_DIR}${EXECUTABLE} ${objects}"
   leave_dir
   success "Succesfully built release."
}

target_modules(){
   target_library
   info "Building modules."
   require_directory $BIN_DIR
   require_directory $MODULES_BIN
   for module in "${MODULES[@]}"
   do
     change_dir $MODULES_DIR
     change_dir $module
     exec "./build.sh all"
     leave_dir
   done
   success "Succesfully built modules."
}



target_all(){
   target_release
   target_modules
}

target_all-debug(){
   target_library-debug
   target_debug
   target_modules
}
if [[ `type -t "target_${1}"` == "function" ]]; then
     eval "target_${1}"
     success "Done."
else
     error "No such target:${1}."
fi
