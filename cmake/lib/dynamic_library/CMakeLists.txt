set(module_root "${lib_root}/dynamic_library")
set(source_root "${module_root}/src/claw/dynamic_library")

claw_library(
  TARGET claw_dynamic_library
  MODULE_ROOT ${module_root}
  SOURCES "${source_root}/dynamic_library.cpp"
  LINK claw_core ${CMAKE_DL_LIBS}
  )
