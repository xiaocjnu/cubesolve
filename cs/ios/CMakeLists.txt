cmake_minimum_required(VERSION 3.20)

project(cs)

message(STATUS "CMAKE_SYSTEM_NAME:" ${CMAKE_SYSTEM_NAME})

# macro
macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY}
    ${XCODE_VALUE})
endmacro (set_xcode_property)

aux_source_directory(../ CS_SRCS)

# release build
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fembed-bitcode")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fembed-bitcode")
set(CMAKE_SUPPRESS_REGENERATION true)

add_definitions(-DNDebug -DIos)
add_library(cs STATIC ${CS_SRCS})

set_xcode_property (cs IPHONEOS_DEPLOYMENT_TARGET "8.3")
set_xcode_property (cs CLANG_CXX_LANGUAGE_STANDARD "gnu++0x")
set_xcode_property (cs CLANG_CXX_LIBRARY "libc++")