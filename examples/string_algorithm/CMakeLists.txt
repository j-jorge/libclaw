cmake_minimum_required(VERSION 2.6)

project(claw-string_algorithm-example)

# check profiling mode
if( CMAKE_BUILD_TYPE MATCHES profile )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -pg" )
endif( CMAKE_BUILD_TYPE MATCHES profile )

# check DEBUG mode
if( CMAKE_BUILD_TYPE MATCHES debug )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG" )
endif( CMAKE_BUILD_TYPE MATCHES debug )

# common flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Wall" )

find_package(libclaw)

include_directories( ${CLAW_INCLUDE_DIRECTORY} )
add_definitions( ${CLAW_DEFINITIONS} )

add_executable( ex-string_algorithm string_algorithm.cpp )
