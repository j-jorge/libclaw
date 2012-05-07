cmake_minimum_required(VERSION 2.6)

project(claw)

include( ${CMAKE_ROOT}/Modules/FindDoxygen.cmake OPTIONAL )

if( DOXYGEN_FOUND )
  add_custom_target(
    doc ALL "${DOXYGEN_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
  )

  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html")

  # install documentation
  install(
    DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html"
    DESTINATION ${CLAW_INSTALLDIR_DOC}
  )
else( DOXYGEN_FOUND )
  message( "doxygen not found." )
endif( DOXYGEN_FOUND )
