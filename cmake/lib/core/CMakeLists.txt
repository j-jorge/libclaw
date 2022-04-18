set(module_root "${lib_root}/core")

add_library(claw_core INTERFACE)

configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/version.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/claw/version.hpp"
  @ONLY
)

target_include_directories(
  claw_core
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/claw/>
  $<BUILD_INTERFACE:${module_root}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

claw_install_library(
  TARGET claw_core
  MODULE_ROOT ${module_root}
  )

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/claw/version.hpp"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/claw"
  )
