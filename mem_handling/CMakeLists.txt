cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 99)

project(mem_handling C)

set(MEM_HANDLING_LIB mem_handler.h mem_handler.c tests.c types.h mem_pool_handler.h mem_pool_handler.c types.h mem_print.h)

set(CMAKE_C_FLAGS_RELEASE "-g3 -O3")

add_library(mem_handling STATIC ${MEM_HANDLING_LIB})

install(TARGETS mem_handling DESTINATION .)