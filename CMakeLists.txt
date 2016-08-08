cmake_minimum_required(VERSION 2.6)
project(bprintf)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(bprintf SHARED src/bprintf.c include/bprintf.h)

add_executable(bprintf-tests src/test.c)
target_link_libraries(bprintf-tests LINK_PUBLIC bprintf)
