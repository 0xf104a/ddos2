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

require_ld_flag(){
    cd /tmp/
    echo "int main(void){ return 0; }" >> /tmp/main.c
    gcc -c /tmp/main.c -o /tmp/main.o
    ld ${1} /tmp/main.o > /dev/null 
    code=$?
    if [ ! $code -eq 0 ]; then
       error "Checking ld has flag ${1}...${red}${bold}FAIL${normal}"
       rm -rf /tmp/main.*
       rm -rf /tmp/a.out
       exit -1
    fi
    info "Checking ld has flag ${1}...${green}${bold}OK${normal}"
    rm -rf /tmp/main.*
    rm -rf /tmp/a.out
    cd $BASEDIR
}

check_ld_flag(){
    cd /tmp/
    echo "int main(void){ return 0; }" >> /tmp/main.c
    gcc -c /tmp/main.c -o /tmp/main.o
    ld ${1} /tmp/main.o >> /dev/null 2>&1
    code=$?
    if [ ! $code -eq 0 ]; then
       info "Checking ld has flag ${1}...${red}${bold}No${normal}"
       rm -rf /tmp/main.*
       rm -rf /tmp/a.out
       cd $BASEDIR
       return -1
    fi
    info "Checking ld has flag ${1}...${green}${bold}Yes${normal}"
    rm -rf /tmp/main.*
    rm -rf /tmp/a.out
    cd $BASEDIR
    return 0
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

source Buildfile

if [[ `type -t "target_${1}"` == "function" ]]; then
     eval "target_${1}"
     success "Done."
else
     error "No such target:${1}."
fi
