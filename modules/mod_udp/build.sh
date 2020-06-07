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
    echo "Usage:"$0" <-h> [all|debug|release|clean|library]"
    echo "-h        Display this help message and exit."
    echo "debug     Build in debug mode"
    echo "release   Build in release mode"
    echo "library   Build libddos2"
    echo "clean     Remove obj/ bin/ directories."
    exit 0
fi

BASEDIR=`pwd`
CC="gcc"
CFLAGS="-c -fPIC -I../../library/include/"
LD="ld"
LD_FLAGS="-ldl"
OBJ_DIR="../../obj/"
BIN_DIR="../../bin/modules/"
LIB_DIR="../../lib/"
EXECUTABLE="mod_udp.so"

declare -a SOURCES=("util" "socket" "interface" "mod_udp")

target_all(){
    info "Building mod_udp."
    require_directory $OBJ_DIR
    require_directory $BIN_DIR
    require_directory $LIB_DIR
    for file in "${SOURCES[@]}"
    do
        exec "${CC} ${CFLAGS} ${file}.c -o ${OBJ_DIR}${file}.o"
    done
    change_dir $OBJ_DIR
    objects=$(printf " %s.o" "${SOURCES[@]}")
    exec "${LD} ${LD_FLAGS} -shared -L${BASEDIR}/${LIB_DIR} -o ${BASEDIR}/${BIN_DIR}${EXECUTABLE} ${objects} -lddos2"
    leave_dir
    success "Succesfully built mod_udp."
}

if [[ `type -t "target_${1}"` == "function" ]]; then
     eval "target_${1}"
     success "Done."
else
     error "No such target:${1}."
fi
