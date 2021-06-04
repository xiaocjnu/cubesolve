cmake_minimum_required(VERSION 3.20)
project(cs)
aux_source_directory(../ CS_SRCS)

message(STATUS "CMAKE_SYSTEM_NAME:" ${CMAKE_SYSTEM_NAME})

# debug build
set(CMAKE_BUILD_TYPE Release)

# add compile options
add_compile_options(-std=c++11 -mfloat-abi=softfp -mfpu=vfp)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

# 添加NDebug宏定义
add_definitions(-DNDebug -DAndroid)
add_library(cs SHARED ${CS_SRCS})