cmake_minimum_required(VERSION 2.6)

project(claw)

subdirs( impl )

file(GLOB CLAW_HEADERS "*.hpp")
install( FILES ${CLAW_HEADERS} DESTINATION "${CLAW_INSTALLDIR_SOURCE}" )
install(
  DIRECTORY meta tween
  DESTINATION "${CLAW_INSTALLDIR_SOURCE}"
  PATTERN "*~" EXCLUDE
  PATTERN ".svn" EXCLUDE
  )
