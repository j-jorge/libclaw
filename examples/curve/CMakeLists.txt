cmake_minimum_required(VERSION 2.6)

project(claw-curve-example)

# common flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Wall" )

#-------------------------------------------------------------------------------
# look for libraries
find_package(libclaw)

add_definitions( ${CLAW_DEFINITIONS} )
include_directories(
  ${CLAW_INCLUDE_DIRECTORY}
  )
link_directories(
  ${CLAW_LINK_DIRECTORY}
  )

add_executable( ex-curve main.cpp )
target_link_libraries( ex-curve
  ${CLAW_CURVE_LIBRARIES}
  ${CLAW_GRAPHIC_LIBRARIES}
  )
