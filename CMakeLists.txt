cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME suitcase)

project(${PROJECT_NAME})

# * Using C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# * Enable -O3 optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# * Static linking
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")

# * Collecting source files
file(GLOB_RECURSE _SOURCES		CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE _SOURCES_C	CONFIGURE_DEPENDS "src/*.c")
file(GLOB_RECURSE _HEADERS		CONFIGURE_DEPENDS "src/*.h")
file(GLOB_RECURSE _HEADERS_HPP	CONFIGURE_DEPENDS "src/*.hpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/libopengl/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/libfreetype/include")

add_executable(${PROJECT_NAME} 	${_SOURCES}
								${_HEADERS}
								${_SOURCES_C}
								${_HEADERS_HPP})

# * Using 6 processor cores for build
add_compile_options(-j 6)

# * Copy dll into build folder
file(GLOB_RECURSE _DLL	CONFIGURE_DEPENDS "src/*.dll")
file(COPY ${_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_SOURCES}
													${_HEADERS}
													${_SOURCES_C}
													${_HEADERS_HPP})

set(_INCLUDE_DIRS "")

foreach(_headerFile ${_HEADERS} ${_HEADERS_HPP})
	get_filename_component(_dir ${_headerFile} PATH)
	list(APPEND _INCLUDE_DIRS ${_dir})
endforeach()

list(REMOVE_DUPLICATES _INCLUDE_DIRS)

target_include_directories(${PROJECT_NAME} PRIVATE ${_INCLUDE_DIRS})

set(MODBUS_LIB   "${CMAKE_CURRENT_LIST_DIR}/src/libmodbus/lib/ws2_32.lib")
set(FREETYPE_LIB "${CMAKE_CURRENT_LIST_DIR}/src/libfreetype/lib/freetyped.lib")

# * Choose OpenGL lib depending on compiler
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(OPENGL_LIB "${CMAKE_CURRENT_LIST_DIR}/src/libopengl/lib/libglfw3.a")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(OPENGL_LIB "${CMAKE_CURRENT_LIST_DIR}/src/libopengl/lib/glfw3_mt.lib")
else()
	message(FATAL_ERROR "Unsupported compiler")
endif()

message(STATUS "Using OpenGL library: ${OPENGL_LIB}")

target_link_libraries(${PROJECT_NAME} 	${MODBUS_LIB}
										${OPENGL_LIB}
										${FREETYPE_LIB})

# * Copying resources
set(_RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/resources")
set(_RESOURCES_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")
file(GLOB_RECURSE _RESOURCES_FILES CONFIGURE_DEPENDS "src/*")
add_custom_target(copy_resources
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${_RESOURCES_BIN_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${_RESOURCES_DIR} ${_RESOURCES_BIN_DIR}
		DEPENDS ${_RESOURCES_FILES}
)

add_dependencies(${PROJECT_NAME} copy_resources)
