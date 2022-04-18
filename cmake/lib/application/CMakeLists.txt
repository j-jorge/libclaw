set(module_root "${lib_root}/application")
set(source_root "${module_root}/src/claw/application")

claw_library(
  TARGET claw_application
  MODULE_ROOT ${module_root}
  SOURCES
    "${source_root}/application.cpp"
    "${source_root}/arguments.cpp"
    "${source_root}/arguments_table.cpp"
  LINK
    claw_logger
  )

if(NOT WIN32)
  set_target_properties(
    claw_application
    PROPERTIES
    COMPILE_DEFINITIONS
    "CLAW_TEXT_DOMAIN_PATH=${CMAKE_INSTALL_PREFIX}/${CLAW_TRANSLATIONS_INSTALL_DIR}"
    )
endif()
