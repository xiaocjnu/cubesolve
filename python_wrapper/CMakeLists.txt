cmake_minimum_required(VERSION 3.13)
project(mycube)


add_definitions(-std=c++14)


include_directories(
	..
	../cs
	../tools
	../snappy
	$ENV{PYTHON_INCLUDE}
)


link_directories(
	$ENV{PYTHON_LIB}
)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

aux_source_directory(../cs CS_SRCS)
aux_source_directory(../tools TOOLS_SRCS)


file(GLOB CS_HDRS ../cs/*.h)
file(GLOB TOOLS_HDRS ../tools/*.h)


add_subdirectory(../snappy snappy)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})           #设置库文件的输出目录
endif()

if (WIN32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")  # 禁用Unicode警告
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}) 


source_group(cs FILES ${CS_SRCS} ${CS_HDRS})
source_group(tools FILES ${TOOLS_SRCS} ${TOOLS_HDRS})

add_library(mycube SHARED
	mycube_wrap.cpp
	${CS_SRCS} ${CS_HDRS}
	${TOOLS_SRCS} ${TOOLS_HDRS}
)

target_link_libraries(mycube snappy)
	

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set_target_properties(mycube PROPERTIES 
		OUTPUT_NAME "_mycube" 
		PREFIX ""
	)
else()
	set_target_properties(mycube PROPERTIES 
		OUTPUT_NAME "_mycube" 
		SUFFIX  ".pyd"
	)
endif()


#add_subdirectory(../cs cs.dir)


#link_libraries(mycube cs)

