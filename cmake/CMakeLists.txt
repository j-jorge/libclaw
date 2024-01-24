cmake_minimum_required(VERSION 3.14)

project(claw VERSION 1.8.0)

include(GNUInstallDirs)

set(repository_root "${CMAKE_CURRENT_LIST_DIR}/..")
set(
  claw_install_documentation_dir
  ${CMAKE_INSTALL_DATADIR}/doc/libclaw${CMAKE_PROJECT_VERSION_MAJOR}
  )

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY
  )
add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

function(claw_install_library)

  cmake_parse_arguments(ARG
    ""
    "TARGET;MODULE_ROOT"
    ""
    ${ARGN}
    )

  install(TARGETS ${ARG_TARGET} EXPORT claw)
  install(
    DIRECTORY "${ARG_MODULE_ROOT}/include/claw"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
endfunction()

function(claw_library)
  cmake_parse_arguments(ARG
    ""
    "TARGET;MODULE_ROOT"
    "SOURCES;LINK"
    ${ARGN}
    )

  add_library(${ARG_TARGET} ${ARG_SOURCES})

  set_target_properties(
    ${ARG_TARGET} PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}
    )

  target_link_libraries(${ARG_TARGET} PUBLIC ${ARG_LINK})

  target_include_directories(
    ${ARG_TARGET}
    PUBLIC
    $<BUILD_INTERFACE:${ARG_MODULE_ROOT}/include>
    )

  claw_install_library(
    TARGET ${ARG_TARGET}
    MODULE_ROOT ${ARG_MODULE_ROOT}
    )
endfunction()

add_subdirectory(lib)

option(CLAW_BUILD_LOCALIZATION "Generate localized files." ON)

if (CLAW_BUILD_LOCALIZATION)
  add_subdirectory(desktop/locale)
endif()

option(CLAW_BUILD_DOCUMENTATION "Generate the documentation." ON)

if (CLAW_BUILD_DOCUMENTATION)
  add_subdirectory(doc)
endif()

option(CLAW_BUILD_EXAMPLES "Build the example programs." ON)

if (CLAW_BUILD_EXAMPLES)
  add_subdirectory(example)
endif()

add_subdirectory(package)
