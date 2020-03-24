cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/powerpc-wrs-linux-gnu-target.cmake")

set(CMAKE_C_COMPILER ${TOOLCHAIN}/powerpc-wrs-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}/powerpc-wrs-linux-gnu-g++)

# set the project name
project(hal)

find_package(PythonLibs 2 REQUIRED)
# find_package(PythonInterp ${PYTHONLIBS_VERSION_STRING} REQUIRED)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

if(PYTHONLIBS_VERSION_STRING MATCHES "^2.*$" )
    set(CMAKE_SWIG_FLAGS -classic)
else()
   set(CMAKE_SWIG_FLAGS -py3)
endif()

set(LIBDL ${CMAKE_SYSROOT}/usr/lib/libdl.so)
set(LIBRT ${CMAKE_SYSROOT}/lib/librt.so.1)
set(LIBPTHREAD ${CMAKE_SYSROOT}/lib/libpthread.so.0)



# cmake warns if loaded from a min-3.0-required parent dir, so silence the warning:
#cmake_policy(SET CMP0078 NEW)
#cmake_policy(SET CMP0086 NEW)



message("PYTHONLIBS_VERSION_STRING: ${PYTHONLIBS_VERSION_STRING}")
message("CMAKE_SWIG_FLAGS: ${CMAKE_SWIG_FLAGS}")
message(${PYTHON_LIBRARIES})
message("${LIBDL}")

# check c++11 support
# if(COMPILER_SUPPORTS_CXX11)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# elseif(COMPILER_SUPPORTS_CXX0X)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
# else()
# message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
# endif()

# no support check, use c++0x without check.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpermissive")


add_custom_target(build_digi_g5 
        COMMAND ${CMAKE_MAKE_PROGRAM} BUILD=customer_performance_m32_bigendian_msbitfirst_gcc_nostrict
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pm6010Drv3/
        COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pm6010Drv3/builds/lib/libg5host.a ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lib
        COMMENT "pm6010drv3")

set(LIBG5HOST_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pm6010Drv3/builds/lib/libg5host.a)
add_library(g5host STATIC IMPORTED)
set_property(TARGET g5host APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(g5host PROPERTIES IMPORTED_LOCATION_NOCONFIG "${LIBG5HOST_LIBRARY}")
add_dependencies(g5host build_digi_g5)

include_directories( ${PYTHON_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pm6010Drv3/host/g5_api/inc)

set_source_files_properties(hal.i PROPERTIES CPLUSPLUS ON)

# set(HALSRC ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)
file(GLOB HALSRC "src/*.cpp")



swig_add_library(hal TYPE SHARED LANGUAGE python SOURCES hal.i ${HALSRC})
#need place another lib before python,otherwise bcm can't find dl function
swig_link_libraries(hal ${LIBDL} ${LIBRT} ${LIBPTHREAD} g5host  ${PYTHON_LIBRARIES} ) 

# add_library(hal SHARED    ${CMAKE_SOURCE_DIR}/src/BcmCli.cpp)
# link_libraries(  ${LIBDL})

add_custom_target(tmp
                COMMAND cp *.so /home/jcai/tmp/
                COMMAND cp *.py /home/jcai/tmp/)


add_executable(test ${HALSRC})
target_link_libraries(test ${LIBDL} ${LIBRT} ${LIBPTHREAD} g5host)


# add_subdirectory(thirdparty)
