cmake_minimum_required(VERSION 3.5)
project(pa1.1)
include_directories(./include)

add_subdirectory(src)
 
set( CMAKE_BUILD_TYPE "Release" )
message(STATUS "Compile type is " ${CMAKE_BUILD_TYPE})

add_executable(main main.c)
target_link_libraries(main add)
