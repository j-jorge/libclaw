cmake_minimum_required(VERSION 2.6)

project(claw-dynamic_library-example)

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

#-------------------------------------------------------------------------------
# look for libraries
find_package(libclaw)

add_definitions( ${CLAW_DEFINITIONS} )
include_directories( ${CLAW_INCLUDE_DIRECTORY} )
link_directories( ${CLAW_LINK_DIRECTORY} )

add_library( func SHARED func.cpp )
add_executable( ex-dynamic_library main.cpp )

target_link_libraries( ex-dynamic_library ${CLAW_DYNAMIC_LIBRARY_LIBRARIES} )
