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

declare -a SOURCES=("message" "util" "dos" "status" "array" "hashtable" "cache" "commons" "network" "module" "arguments" "main")
declare -a MODULES=("mod_a" "mod_udp")

cd $BASEDIR

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
       exec "${CC} -c ${CFLAGS} -fsanitize=leak -fsanitize=address -fsanitize=undefined src/${file}.c -o ${OBJ_DIR}${file}.o"
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
       exec "${CC} -c ${CFLAGS} -Ofast src/${file}.c -o ${OBJ_DIR}${file}.o"
   done
   change_dir $OBJ_DIR
   objects=$(printf " %s.o" "${SOURCES[@]}")
   exec "${LD} ${LD_FLAGS} -o ${BASEDIR}/${BIN_DIR}${EXECUTABLE} ${objects}"
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

target_test(){
   target_all
   exec "./bin/ddos2 --module mod_a --test"
}
