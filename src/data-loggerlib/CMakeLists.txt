# DataLoggerlib

cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME DataLoggerlib)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

project(${PROJECT_NAME})

file (GLOB_RECURSE _SOURCES 	CONFIGURE_DEPENDS "*.cpp")
file (GLOB_RECURSE _HEADERS 	CONFIGURE_DEPENDS "*.h")


add_library(${PROJECT_NAME} STATIC ${_SOURCES} ${_HEADERS})


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_SOURCES} ${_HEADERS})


set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "data-loggerlib")

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION "${CMAKE_CURRENT_LIST_DIR}/lib"
        ARCHIVE DESTINATION "${CMAKE_CURRENT_LIST_DIR}/lib")
        
install(FILES data-logger.h DESTINATION include/)