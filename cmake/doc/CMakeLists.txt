find_package(Doxygen)

if(NOT DOXYGEN_FOUND)
  message(WARNING "Could not find Doxygen, skipping documentation.")
  return()
endif()

set(DOXYGEN_DOT_MULTI_TARGETS YES)
set(DOXYGEN_EXAMPLE_PATH ${repository_root}/example)
set(DOXYGEN_IMAGE_PATH ${repository_root}/doc/doc-img)
set(DOXYGEN_SOURCE_BROWSER YES)
set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)

doxygen_add_docs(devdoc
  ${repository_root}/lib/
  ${repository_root}/doc/doc-src/
  ALL
  WORKING_DIRECTORY ${repository_root}
  )

install(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
  DESTINATION ${claw_install_documentation_dir}
  )
