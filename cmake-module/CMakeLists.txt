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

#-------------------------------------------------------------------------------
# version file
set(
  CLAW_VERSION_FILE_NAME
  "${CMAKE_CURRENT_BINARY_DIR}/libclaw-config-version.cmake"
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CLAW_VERSION_FILE_NAME}"
  VERSION "${CLAW_VERSION}"
  COMPATIBILITY SameMajorVersion
  )

install(
  FILES "${CLAW_VERSION_FILE_NAME}"
  DESTINATION "${CLAW_CMAKE_MODULES_INSTALL_PATH}"
  )
