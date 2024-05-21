cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME suitcase)

# TODO: This file needs a complex refactoring

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect source files
file (GLOB_RECURSE _SOURCES 	CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE _SOURCES_C 	CONFIGURE_DEPENDS "src/*.c")
file (GLOB_RECURSE _HEADERS 	CONFIGURE_DEPENDS "src/*.h")
file (GLOB_RECURSE _HEADERS_HPP CONFIGURE_DEPENDS "src/*.hpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/libopengl/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/libfreetype/include")

add_executable(${PROJECT_NAME} 	${_SOURCES}
		${_HEADERS}
		${_SOURCES_C}
		${_HEADERS_HPP})



source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_SOURCES}
		${_HEADERS}
		${_SOURCES_C}
		${_HEADERS_HPP})

set(_INCLUDE_DIRS "")

foreach (_headerFile ${_HEADERS} ${_HEADERS_HPP})
	get_filename_component(_dir ${_headerFile} PATH)
	list (APPEND _INCLUDE_DIRS ${_dir})
endforeach()

list(REMOVE_DUPLICATES _INCLUDE_DIRS)

target_include_directories(${PROJECT_NAME} PRIVATE ${_INCLUDE_DIRS})

set(MODBUS_LIB   "${CMAKE_CURRENT_LIST_DIR}/src/libmodbus/lib/ModBus.lib")
set(FREETYPE_LIB "${CMAKE_CURRENT_LIST_DIR}/src/libfreetype/lib/freetyped.lib")

# Choose OpenGL lib depending on compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/libfreetype/lib/freetyped.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/libmodbus/dll/ModBus.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	set(OPENGL_LIB "${CMAKE_CURRENT_LIST_DIR}/src/libopengl/lib/libglfw3.a")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	file(MAKE_DIRECTORY "./build/Debug")
	file(MAKE_DIRECTORY "./build/Release")
	set(OPENGL_LIB "${CMAKE_CURRENT_LIST_DIR}/src/libopengl/lib/glfw3_mt.lib")
	file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/libfreetype/lib/freetyped.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
	file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/libmodbus/dll/ModBus.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
	file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/libfreetype/lib/freetyped.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
	file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/libmodbus/dll/ModBus.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
else ()
	message(FATAL_ERROR "Unsupported compiler")
endif()

message(STATUS "Using OpenGL library: ${OPENGL_LIB}")

target_link_libraries(${PROJECT_NAME} 	${MODBUS_LIB}
		${OPENGL_LIB}
		${FREETYPE_LIB})