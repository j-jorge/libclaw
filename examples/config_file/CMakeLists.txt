cmake_minimum_required(VERSION 2.6)

project(claw-config_file-example)

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

add_executable( ex-config main.cpp application.cpp )
target_link_libraries(
  ex-config
  ${CLAW_CONFIGURATION_FILE_LIBRARIES}
  ${CLAW_APPLICATION_LIBRARIES} )
