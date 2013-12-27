##
## This module finds if the CLAW (C++ Library Absolutely Wonderful) is
## installed and determines where the include files and libraries
## are. It also determines what the name of the library is. This code
## sets the following variables:
##
## ------------------------------------------------------------------
##
## CLAW_FOUND             = System has the claw library.
##
## CLAW_DEFINITIONS       = Compiler definitions (-D).
##
## CLAW_INCLUDE_DIRECTORY = Where to find headers "claw/claw.hpp".
##
## CLAW_LINK_DIRECTORY    = Link directory.
##
## CLAW_LIBRARIES         = Claw libraries to link to.
##
## CLAW_MAJOR_VERSION     = The major version number of the library.
##
## CLAW_MINOR_VERSION     = The minor version number of the library.
##
## CLAW_RELEASE_NUMBER    = The release number of this version of the library.
##
## CLAW_REVISION_NUMBER   = The SVN revision number of the library. To be used
##                          only when developping with a not yet released
##                          version of the library.
##
## CLAW_VERSION           = The full version of the library, as
##                          Major.Minor.Release.
##
## ------------------------------------------------------------------
##
## Author Julien Jorge (10/2010)
##
## ------------------------------------------------------------------
## 

IF( CLAW_FOUND )
  SET(CLAW_FIND_QUIETLY TRUE)
ENDIF( CLAW_FOUND )

IF (UNIX) 
  ## ######################################################################
  ## 
  ## UNIX/Linux specific:
  ## 
  ## use backquoted claw-config to query and set flags and libs
  ## 
  
  STRING(REPLACE : " " SEARCH_PATH $ENV{PATH})

  # claw-config should be in your path anyhow
  FIND_PROGRAM(CMAKE_CLAW_CLAWCONFIG_EXECUTABLE claw-config ${SEARCH_PATH})
  
  # check wether claw-config was found:
  IF(CMAKE_CLAW_CLAWCONFIG_EXECUTABLE)    

    # set CXXFLAGS to be fed into CMAKE_DEFINITIONS by the user:
    EXEC_PROGRAM( ${CMAKE_CLAW_CLAWCONFIG_EXECUTABLE}
      ARGS "--cxxflags"
      OUTPUT_VARIABLE CLAW_DEFINITIONS )

    # evaluate claw-config output to separate linker flags and linkdirs for
    # rpath:
    EXEC_PROGRAM(${CMAKE_CLAW_CLAWCONFIG_EXECUTABLE}
      ARGS "--libs"
      OUTPUT_VARIABLE CLAW_CONFIG_LINK_DIRECTORY_WITH_PREFIX )
    
    ## extract linkdirs (-L)
    ## use regular expression to match wildcard equivalent "-L*<endchar>"
    ## with <endchar> is a space or a semicolon
    STRING( REGEX REPLACE
      "[-][L](.+)$" "\\1"
      CLAW_LINK_DIRECTORY
      "${CLAW_CONFIG_LINK_DIRECTORY_WITH_PREFIX}" )
    
    # set CXXFLAGS to be fed into CMAKE_DEFINITIONS by the user:
    EXEC_PROGRAM( ${CMAKE_CLAW_CLAWCONFIG_EXECUTABLE}
      ARGS "--include"
      OUTPUT_VARIABLE CLAW_CONFIG_INCLUDE_DIRECTORIES_WITH_PREFIX )

    ## extract include dirs (-I) for rpath
    ## use regular expression to match wildcard equivalent "-I*<endchar>"
    ## with <endchar> is a space or a semicolon
    STRING( REGEX REPLACE
      "[-][I](.+)$" "\\1"
      CLAW_INCLUDE_DIRECTORY
      "${CLAW_CONFIG_INCLUDE_DIRECTORIES_WITH_PREFIX}" )
    
    # we really need claw-config...
  ELSE(CMAKE_CLAW_CLAWCONFIG_EXECUTABLE)    
    MESSAGE(SEND_ERROR "Cannot find claw-config anywhere on the system. Please put the file into your path or specify it in CMAKE_CLAW_CLAWCONFIG_EXECUTABLE.")
  ENDIF(CMAKE_CLAW_CLAWCONFIG_EXECUTABLE)

  MARK_AS_ADVANCED(CMAKE_CLAW_CLAWCONFIG_EXECUTABLE)
ENDIF(UNIX)

IF(WIN32)
  FIND_PATH(CLAW_ROOT_DIRECTORY "include/claw/application.hpp"
    "C:/claw/"
    "D:/claw/"
    "C:/Program Files/claw/"
    "D:/Program Files/claw/"
    )

  SET( CLAW_INCLUDE_DIRECTORY "${CLAW_ROOT_DIRECTORY}/include" )
  SET( CLAW_LINK_DIRECTORY "${CLAW_ROOT_DIRECTORY}/lib" )

  FIND_PATH( CLAW_CONFIG_WIN_CXXFILE "win/definitions.txt"
    "${CLAW_ROOT_DIRECTORY}" )

  IF( CLAW_CONFIG_WIN_CXXFILE )
    FILE( READ "${CLAW_CONFIG_WIN_CXXFILE}/win/definitions.txt"
      CLAW_DEFINITIONS )
  ENDIF( CLAW_CONFIG_WIN_CXXFILE )
ENDIF(WIN32)

IF( CLAW_INCLUDE_DIRECTORY
    AND CLAW_LINK_DIRECTORY
    AND CLAW_DEFINITIONS
    )
  SET(CLAW_FOUND 1)
ELSE( CLAW_INCLUDE_DIRECTORY AND CLAW_LINK_DIRECTORY AND CLAW_DEFINITIONS )
  SET(CLAW_FOUND 0)
ENDIF( CLAW_INCLUDE_DIRECTORY AND CLAW_LINK_DIRECTORY AND CLAW_DEFINITIONS )

#-------------------------------------------------------------------------------
# Boost is required to build libclaw
FIND_PACKAGE( Boost 1.42 REQUIRED COMPONENTS system )

IF( NOT Boost_FOUND )
  MESSAGE( FATAL_ERROR "Boost.system (>=1.42) is required to use libclaw." )
  SET( CLAW_FOUND 0 )
ENDIF( NOT Boost_FOUND )

#-------------------------------------------------------------------------------
# claw_logger
FIND_LIBRARY(
  CLAW_LOGGER_LIB
  NAMES claw_logger
  PATHS ${CLAW_LINK_DIRECTORY} )

IF( NOT CLAW_LOGGER_LIBRARIES )
  MESSAGE( FATAL_ERROR "Could not find claw_logger library" )
  SET(CLAW_FOUND 0)
ELSE( NOT CLAW_LOGGER_LIBRARIES )
  SET(
    CLAW_LOGGER_LIBRARIES
    ${CLAW_LOGGER_LIB}
    ${Boost_SYSTEM_LIBRARY} )
ENDIF( NOT CLAW_LOGGER_LIBRARIES )

#-------------------------------------------------------------------------------
# claw_application
FIND_LIBRARY(
  CLAW_APPLICATION_LIB
  NAMES claw_application
  PATHS ${CLAW_LINK_DIRECTORY} )

IF( NOT CLAW_APPLICATION_LIB )
  MESSAGE( FATAL_ERROR "Could not find claw_application library" )
  SET(CLAW_FOUND 0)
ELSE( NOT CLAW_APPLICATION_LIB )
  SET(
    CLAW_APPLICATION_LIBRARIES
    ${CLAW_APPLICATION_LIB}
    ${CLAW_LOGGER_LIBRARIES} )
ENDIF( NOT CLAW_APPLICATION_LIB )

#-------------------------------------------------------------------------------
# claw_configuration_file
FIND_LIBRARY(
  CLAW_CONFIGURATION_FILE_LIBRARIES
  NAMES claw_configuration_file
  PATHS ${CLAW_LINK_DIRECTORY} )

IF( NOT CLAW_CONFIGURATION_FILE_LIBRARIES )
  MESSAGE( FATAL_ERROR "Could not find claw_configuration_file library" )
  SET(CLAW_FOUND 0)
ENDIF( NOT CLAW_CONFIGURATION_FILE_LIBRARIES )

#-------------------------------------------------------------------------------
# claw_dynamic_library
FIND_LIBRARY(
  CLAW_DYNAMIC_LIBRARY_LIB
  NAMES claw_dynamic_library
  PATHS ${CLAW_LINK_DIRECTORY} )

IF( NOT CLAW_DYNAMIC_LIBRARY_LIB )
  MESSAGE( FATAL_ERROR "Could not find claw_dynamic_library library" )
  SET(CLAW_FOUND 0)
ELSE( NOT CLAW_DYNAMIC_LIBRARY_LIB )

  SET(
    CLAW_DYNAMIC_LIBRARY_LIBRARIES
    ${CLAW_DYNAMIC_LIBRARY_LIB} )

  IF(UNIX)
      SET(
        CLAW_DYNAMIC_LIBRARY_LIBRARIES
        ${CLAW_DYNAMIC_LIBRARY_LIBRARIES}
        ${CMAKE_DL_LIBS} )
  ENDIF(UNIX)

ENDIF( NOT CLAW_DYNAMIC_LIBRARY_LIB )

#-------------------------------------------------------------------------------
# claw_graphic
FIND_LIBRARY(
  CLAW_GRAPHIC_LIB
  NAMES claw_graphic
  PATHS ${CLAW_LINK_DIRECTORY} )

IF( NOT CLAW_GRAPHIC_LIB )
  MESSAGE( FATAL_ERROR "Could not find claw_graphic library" )
  SET(CLAW_FOUND 0)
ELSE( NOT CLAW_GRAPHIC_LIB )

  SET( CLAW_GRAPHIC_LIBRARIES ${CLAW_GRAPHIC_LIB} )

  STRING( REGEX MATCH "CLAW_JPEG_SUPPORT"
    CLAW_CONFIG_USES_JPEG
    "${CLAW_DEFINITIONS}" )
  STRING( REGEX MATCH "CLAW_PNG_SUPPORT"
    CLAW_CONFIG_USES_PNG
    "${CLAW_DEFINITIONS}" )

  IF ( CLAW_CONFIG_USES_JPEG )
    INCLUDE(FindJPEG)

    IF( JPEG_FOUND )
      SET(
        CLAW_GRAPHIC_LIBRARIES
        ${CLAW_GRAPHIC_LIBRARIES}
        ${JPEG_LIBRARIES} )
      SET( CLAW_GRAPHIC_INCLUDE_DIRECTORY
        ${CLAW_GRAPHIC_INCLUDE_DIRECTORY}
        ${JPEG_INCLUDE_DIR} )
      SET( CLAW_GRAPHIC_LINK_DIRECTORY
        ${CLAW_GRAPHIC_LINK_DIRECTORY}
        ${JPEG_LIBRARY} )
    ELSE( JPEG_FOUND )
      SET( CLAW_FOUND 0 )
    ENDIF( JPEG_FOUND )
  ENDIF( CLAW_CONFIG_USES_JPEG )

  IF ( CLAW_CONFIG_USES_PNG )
    INCLUDE(FindPNG)

    IF( PNG_FOUND )
      SET(
        CLAW_GRAPHIC_LIBRARIES
        ${CLAW_GRAPHIC_LIBRARIES}
        ${PNG_LIBRARIES} )
      SET( CLAW_GRAPHIC_INCLUDE_DIRECTORY
        ${CLAW_GRAPHIC_INCLUDE_DIRECTORY}
        ${PNG_INCLUDE_DIR} )
      SET( CLAW_GRAPHIC_LINK_DIRECTORY
        ${CLAW_GRAPHIC_LINK_DIRECTORY}
        ${PNG_LIBRARY} )
    ELSE( PNG_FOUND )
      SET( CLAW_FOUND 0 )
    ENDIF( PNG_FOUND )
  ENDIF( CLAW_CONFIG_USES_PNG )

ENDIF( NOT CLAW_GRAPHIC_LIB )

#-------------------------------------------------------------------------------
# claw_net
FIND_LIBRARY(
  CLAW_NET_LIB
  NAMES claw_net
  PATHS ${CLAW_LINK_DIRECTORY} )

IF( NOT CLAW_NET_LIB )
  MESSAGE( FATAL_ERROR "Could not find claw_net library" )
  SET(CLAW_FOUND 0)
ELSE( NOT CLAW_NET_LIB )

  SET(
    CLAW_NET_LIBRARIES
    ${CLAW_NET_LIB} )

  IF(WIN32)
    FIND_LIBRARY(
      CLAW_WSOCK32_LIB
      NAMES wsock32 )

    IF( NOT CLAW_WSOCK32_LIB )
      MESSAGE( FATAL_ERROR "Could not find wsock32 library" )
      SET(CLAW_FOUND 0)
    ELSE( NOT CLAW_WSOCK32_LIB )
      SET(
        CLAW_NET_LIBRARIES
        ${CLAW_NET_LIBRARIES}
        ${CLAW_WSOCK32_LIB} )
    ENDIF( NOT CLAW_WSOCK32_LIB )
  ENDIF(WIN32)

ENDIF( NOT CLAW_NET_LIB )

#-------------------------------------------------------------------------------
# claw_tween
FIND_LIBRARY(
  CLAW_TWEEN_LIB
  NAMES claw_tween
  PATHS ${CLAW_LINK_DIRECTORY} )

IF( NOT CLAW_TWEEN_LIB )
  MESSAGE( FATAL_ERROR "Could not find claw_tween library" )
  SET(CLAW_FOUND 0)
ELSE( NOT CLAW_TWEEN_LIB )

  SET( CLAW_TWEEN_LIBRARIES ${CLAW_TWEEN_LIB} )
ENDIF( NOT CLAW_TWEEN_LIB )

#-------------------------------------------------------------------------------
MARK_AS_ADVANCED(
  CLAW_DEFINITIONS
  CLAW_INCLUDE_DIRECTORY
  CLAW_LINK_DIRECTORY

  CLAW_APPLICATION_LIB
  CLAW_DYNAMIC_LIBRARY_LIB
  CLAW_GRAPHIC_LIB
  CLAW_NET_LIB

  CLAW_WSOCK32_LIB

  CLAW_CONFIG_USES_JPEG
  CLAW_CONFIG_USES_PNG

  CLAW_GRAPHIC_INCLUDE_DIRECTORY
  CLAW_GRAPHIC_LINK_DIRECTORY

  CLAW_LOGGER_LIBRARIES
  CLAW_APPLICATION_LIBRARIES
  CLAW_CONFIGURATION_FILE_LIBRARIES
  CLAW_DYNAMIC_LIBRARY_LIBRARIES
  CLAW_GRAPHIC_LIBRARIES
  CLAW_NET_LIBRARIES
  )

IF( CLAW_FOUND )
  SET(CLAW_MAJOR_VERSION 0)
  SET(CLAW_MINOR_VERSION 0)
  SET(CLAW_RELEASE_NUMBER 0)
  SET(CLAW_REVISION_NUMBER 0)
  FILE(
    READ "${CLAW_INCLUDE_DIRECTORY}/claw/version.hpp"
    _CLAW_VERSION_HPP_CONTENTS
    )
  
  STRING(
    REGEX REPLACE ".*#define CLAW_MAJOR_VERSION ([0-9]+).*" "\\1"
    CLAW_MAJOR_VERSION "${_CLAW_VERSION_HPP_CONTENTS}"
    )
  STRING(
    REGEX REPLACE ".*#define CLAW_MINOR_VERSION ([0-9]+).*" "\\1"
    CLAW_MINOR_VERSION "${_CLAW_VERSION_HPP_CONTENTS}"
    )
  STRING(
    REGEX REPLACE ".*#define CLAW_RELEASE_NUMBER ([0-9]+).*" "\\1"
    CLAW_RELEASE_NUMBER "${_CLAW_VERSION_HPP_CONTENTS}"
    )
  STRING(
    REGEX REPLACE ".*#define CLAW_REVISION_NUMBER ([0-9]+).*" "\\1"
    CLAW_REVISION_NUMBER "${_CLAW_VERSION_HPP_CONTENTS}"
    )

  SET(
    CLAW_VERSION
    "${CLAW_MAJOR_VERSION}.${CLAW_MINOR_VERSION}.${CLAW_RELEASE_NUMBER}"
    )

  IF( NOT CLAW_FIND_QUIETLY )
    MESSAGE(STATUS "Found Claw library version ${CLAW_VERSION}")
  ENDIF( NOT CLAW_FIND_QUIETLY )
ELSE( CLAW_FOUND )
  MESSAGE(FATAL_ERROR "Could not find Claw library")
ENDIF( CLAW_FOUND )
