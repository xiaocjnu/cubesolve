# runtime: windows
# depends: mingw64

cmake_minimum_required(VERSION 3.20)
project(cs)
aux_source_directory(../ CS_SRCS)

message(STATUS CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME})

# debug build
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
# set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
# set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

# 添加NDebug宏定义
# add_definitions(-DNDebug -DWindows)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
add_definitions(-DNDebug)
add_library(cs SHARED ${CS_SRCS})
# target_link_options(cs PUBLIC -static-libgcc -static-libstdc++ -lpthread)
message(STATUS COMPILE_OPTIONS: ${COMPILE_OPTIONS})

# target_link_libraries(cs PRIVATE DbgHelp)
add_executable(csmain ${CS_SRCS})
# target_link_libraries(csmain PRIVATE DbgHelp)
