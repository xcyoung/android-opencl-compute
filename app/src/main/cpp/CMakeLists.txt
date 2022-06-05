cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/CL)

file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/cl_stub.c)

add_library( # Sets the name of the library.
        gpu_compute
        SHARED
        ${src-files}
        JNI.cpp)

#target_link_libraries(
#        opencl_compute
#        libcl_stub
#        android
#        log)

target_link_libraries(
        gpu_compute
        android
        log)