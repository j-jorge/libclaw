install(
  EXPORT claw
  DESTINATION lib/cmake/claw/
  FILE claw-config-generated.cmake
  )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/claw-config-version.cmake
  VERSION ${CMAKE_PROJECT_VERSION}
  COMPATIBILITY SameMinorVersion)

configure_file(
  claw-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/claw-config.cmake
  @ONLY )

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/claw-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/claw-config-version.cmake
  DESTINATION lib/cmake/claw/
  )


