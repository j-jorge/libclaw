set(module_root "${lib_root}/logger")
set(source_root "${module_root}/src/claw/logger")

find_package(Intl REQUIRED)

claw_library(
  TARGET claw_logger
  MODULE_ROOT ${module_root}
  SOURCES
    "${source_root}/android_logger.cpp"
    "${source_root}/logger.cpp"
    "${source_root}/log_stream.cpp"
    "${source_root}/log_stream_concise.cpp"
    "${source_root}/log_stream_uniq.cpp"
    "${source_root}/log_level.cpp"
  LINK
    claw_core
    ${Intl_LIBRARIES}
  )
target_compile_features(claw_logger INTERFACE cxx_std_11)
