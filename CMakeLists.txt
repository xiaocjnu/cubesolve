
#add_definitions(-std=c++14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")


option(USE_SNAPPY "option for use snappy" OFF)

if(USE_SNAPPY)
add_definitions(-D USE_SNAPPY)
endif()


aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/cs CS_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/cube_cv CUBE_CV_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ktx/src KTX_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/es_utils/Source ES_UTILS_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/my_model MY_MODEL_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tools TOOLS_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ai_teaching AI_TEACHING_SRCS)

if(USE_SNAPPY)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/snappy SNAPPY_SRCS)
endif()


file(GLOB CS_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/cs/*.h)
file(GLOB CUBE_CV_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/cube_cv/*.h)
file(GLOB KTX_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/ktx/include/*.h)
file(GLOB ES_UTILS_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/es_utils/include/*.h)
file(GLOB MY_MODEL_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/my_model/*.h)
file(GLOB TOOLS_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/tools/*.h)
file(GLOB AI_TEACHING_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/ai_teaching/*.h)



include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/cs
	${CMAKE_CURRENT_SOURCE_DIR}/cube_cv
	${CMAKE_CURRENT_SOURCE_DIR}/my_model
	${CMAKE_CURRENT_SOURCE_DIR}/tools
	${CMAKE_CURRENT_SOURCE_DIR}/ai_teaching
	
	${CMAKE_CURRENT_SOURCE_DIR}/ktx/include
	${CMAKE_CURRENT_SOURCE_DIR}/es_utils/include
	${CMAKE_CURRENT_SOURCE_DIR}/gles/include
)

if(USE_SNAPPY)
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/snappy
)
endif()


add_definitions(-D KTX_OPENGL_ES3)


if(WIN32)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/es_utils/src_win32 ES_UTILS_SRCS_PLATFORM)
endif()


file(GLOB_RECURSE CS_SRCS2     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/cs *.cpp *.h)

source_group(cs   FILES ${CS_SRCS2})

source_group(cs FILES ${CS_SRCS} ${CS_HDRS})
source_group(cube_cv FILES ${CUBE_CV_SRCS} ${CUBE_CV_HDRS})
source_group(ktx FILES ${KTX_SRCS} ${KTX_HDRS})
source_group(es_utils FILES ${ES_UTILS_SRCS} ${ES_UTILS_SRCS_PLATFORM} ${ES_UTILS_HDRS})
source_group(my_model FILES ${MY_MODEL_SRCS} ${MY_MODEL_HDRS})
source_group(tools FILES ${TOOLS_SRCS} ${TOOLS_HDRS})
source_group(ai_teaching FILES ${AI_TEACHING_SRCS} ${AI_TEACHING_HDRS})

if (USE_SNAPPY)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/snappy)
source_group(snappy FILES ${SNAPPY_SRCS})
endif()



add_library(cube 
	${CS_SRCS} ${CS_HDRS}
	${CUBE_CV_SRCS} ${CUBE_CV_HDRS}
	${KTX_SRCS} ${KTX_HDRS}
	${ES_UTILS_SRCS} ${ES_UTILS_HDRS}
	${ES_UTILS_SRCS_PLATFORM} ${ES_UTILS_HDRS}
	${MY_MODEL_SRCS} ${MY_MODEL_HDRS}
	${TOOLS_SRCS} ${TOOLS_HDRS}
	${AI_TEACHING_SRCS} ${AI_TEACHING_HDRS}
	
)



