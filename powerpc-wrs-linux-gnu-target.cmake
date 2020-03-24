# mkdir build; cd build
# cmake -DCMAKE_TOOLCHAIN_FILE= ../../../../../../powerpc-wrs-linux-gnu-target.cmake ..

# where is the target environment 
#SET(CMAKE_FIND_ROOT_PATH ...)

SET(CMAKE_CXX_FLAGS "-m32")

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ppc)
set(home /home/jcai/tlabqoriq50)
#set(CMAKE_SYSROOT $ENV{WORKING_DIR}/$ENV{REPO_METRO}/vobs/ots/wrlplat/tlabqoriq50_small/build/rootfs/sdk/sysroots/tlabqoriqsmall-wrs-linux)
set(CMAKE_SYSROOT ${home}/sysroots/tlabqoriqsmall-wrs-linux)
set(CMAKE_FIND_ROOT_PATH ${home}/sysroots/tlabqoriqsmall-wrs-linux)


#set(TOOLCHAIN $ENV{WORKING_DIR}/$ENV{REPO_METRO}/vobs/ots/wrlplat/tlabqoriq50_small/build/rootfs/sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/ppce500mc-wrs-linux)
set(TOOLCHAIN ${home}/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/ppce500mc-wrs-linux)

LIST(APPEND CMAKE_PROGRAM_PATH ${TOOLCHAIN} ...)

set(CMAKE_C_COMPILER ${TOOLCHAIN}/powerpc-wrs-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}/powerpc-wrs-linux-gnu-g++)

add_definitions(-DLINUX -DWRLINUX -DPPC )

#set(CMAKE_C_COMPILER ${TOOLCHAIN}/ppce500mc-wrswrap-linux-gnu-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLCHAIN}/ppce500mc-wrswrap-linux-gnu-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

