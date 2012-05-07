configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/definitions.txt.in"
  "${CMAKE_CURRENT_BINARY_DIR}/definitions.txt"
  IMMEDIATE @ONLY)

install( FILES definitions.txt
  DESTINATION ${CMAKE_INSTALL_PREFIX}/win )