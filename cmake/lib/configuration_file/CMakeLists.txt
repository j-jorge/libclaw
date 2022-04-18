set(module_root "${lib_root}/configuration_file")
set(source_root "${module_root}/src/claw/configuration_file")

claw_library(
  TARGET claw_configuration_file
  MODULE_ROOT ${module_root}
  SOURCES "${source_root}/configuration_file.cpp"
  LINK claw_core
  )
