cmake_minimum_required(VERSION 2.6)

project(claw:cmake-modules)

if( NOT CLAW_CMAKE_MODULES_INSTALL_PATH )
  set( CLAW_CMAKE_MODULES_INSTALL_PATH "share/cmake/libclaw" )
endif( NOT CLAW_CMAKE_MODULES_INSTALL_PATH )

set( CLAW_CMAKE_MODULES
  libclaw-config.cmake
  )

#-------------------------------------------------------------------------------
# installation
install(
  FILES ${CLAW_CMAKE_MODULES}
  DESTINATION "${CLAW_CMAKE_MODULES_INSTALL_PATH}" )

