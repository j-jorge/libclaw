set(module_root "${lib_root}/net")
set(source_root "${module_root}/src/claw/net")

claw_library(
  TARGET claw_net
  MODULE_ROOT ${module_root}
  SOURCES
    "${source_root}/basic_socket.cpp"
    "${source_root}/socket_server.cpp"
  LINK claw_core
  )

if(WIN32)
  target_link_libraries(claw_net wsock32)
endif()
