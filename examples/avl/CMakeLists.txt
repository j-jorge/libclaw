cmake_minimum_required(VERSION 2.6)

project(claw-avl-example)

# common flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Wall" )

find_package(libclaw)

include_directories( ${CLAW_INCLUDE_DIRECTORY} )
add_definitions( ${CLAW_DEFINITIONS} )

add_executable( ex-avl main.cpp )
add_executable( ex-vs std_vs_claw.cpp )
