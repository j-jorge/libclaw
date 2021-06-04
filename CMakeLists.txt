cmake_minimum_required(VERSION 2.6)

IF(CMAKE_COMPILER_IS_GNUCXX)
  ADD_DEFINITIONS(-Wall -ansi -pedantic)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

project(claw)

include_directories(.)

set( CLAW_VERSION_MAJOR 1 )
set( CLAW_VERSION_MINOR 7 )
set( CLAW_RELEASE_NUMBER 4 )

set( CLAW_VERSION
  ${CLAW_VERSION_MAJOR}.${CLAW_VERSION_MINOR}.${CLAW_RELEASE_NUMBER} 
  )

set(CLAW_REVISION_NUMBER 0)

set( CLAW_CODE_DIR claw/code/ )
set( CLAW_INCLUDE_DIRECTORY "${CMAKE_INSTALL_PREFIX}/include/" )
set( CLAW_INSTALLDIR_SOURCE include/claw/ )
set( CLAW_INSTALLDIR_LIB "${CMAKE_INSTALL_LIBDIR}/" )
set( CLAW_INSTALLDIR_DOC share/doc/libclaw${CLAW_VERSION_MAJOR}/ )
set( CLAW_TRANSLATIONS_INSTALL_DIR "share/locale" )
set( CLAW_EXECUTABLE_DIR bin/ )

set( CLAW_LIBRARIES
  claw_application
  claw_configuration_file
  claw_dynamic_library
  claw_graphic
  claw_logger
  claw_net
  claw_tween
  )

# custom build options
option( CLAW_NO_JPEG
        "Turn off jpeg support"
        OFF )
option( CLAW_NO_PNG
        "Turn off png support"
        OFF )
option( CLAW_INSTALL_CMAKE_MODULES
        "Install cmake modules for CLAW"
        ON )

#-------------------------------------------------------------------------------
# look for the jpeg library
if( CLAW_NO_JPEG )
else( CLAW_NO_JPEG )
  include( ${CMAKE_ROOT}/Modules/FindJPEG.cmake )
  set( CLAW_DEFINITIONS ${CLAW_DEFINITIONS} -DCLAW_JPEG_SUPPORT )
  include_directories(${JPEG_INCLUDE_DIR})
endif( CLAW_NO_JPEG )

# look for the png library
if( CLAW_NO_PNG )
else( CLAW_NO_PNG )
  include( ${CMAKE_ROOT}/Modules/FindPNG.cmake )
  set( CLAW_DEFINITIONS ${CLAW_DEFINITIONS} -DCLAW_PNG_SUPPORT )
  include_directories(${PNG_INCLUDE_DIR})
endif( CLAW_NO_PNG )

# look for boost
include(FindBoost)

find_package( Boost 1.42 REQUIRED COMPONENTS thread system )

if( NOT Boost_FOUND )
  message( FATAL_ERROR "Can't find Boost library on version greater or equal to 1.42 on your system." )
endif( NOT Boost_FOUND )

#-------------------------------------------------------------------------------
# Include directories for Boost
include_directories(
  ${Boost_INCLUDE_DIR}
  )

#-------------------------------------------------------------------------------
# set compilation defines
add_definitions( ${CLAW_DEFINITIONS} )

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++03")

#-------------------------------------------------------------------------------
# application
set( CLAW_APPLICATION_SOURCES
  ${CLAW_CODE_DIR}application.cpp
  ${CLAW_CODE_DIR}arguments.cpp
  ${CLAW_CODE_DIR}arguments_table.cpp
)
add_library( claw_application SHARED ${CLAW_APPLICATION_SOURCES} )
add_library( claw_application-static STATIC ${CLAW_APPLICATION_SOURCES} )
target_link_libraries( claw_application claw_logger )

if( NOT WIN32 )
  set_target_properties(
    claw_application
    PROPERTIES
    COMPILE_DEFINITIONS "CLAW_TEXT_DOMAIN_PATH=${CMAKE_INSTALL_PREFIX}/${CLAW_TRANSLATIONS_INSTALL_DIR}"
    )
  set_target_properties(
    claw_application-static
    PROPERTIES
    COMPILE_DEFINITIONS "CLAW_TEXT_DOMAIN_PATH=${CMAKE_INSTALL_PREFIX}/${CLAW_TRANSLATIONS_INSTALL_DIR}"
    )
endif( NOT WIN32 )

# configuration file
set( CLAW_CONFIGURATION_FILE_SOURCES
  ${CLAW_CODE_DIR}configuration_file.cpp
)
add_library( claw_configuration_file SHARED ${CLAW_CONFIGURATION_FILE_SOURCES} )
add_library(
  claw_configuration_file-static STATIC
  STATIC ${CLAW_CONFIGURATION_FILE_SOURCES}
)

# dynamic libraries
set( CLAW_DYNAMIC_LIBRARY_SOURCES
  ${CLAW_CODE_DIR}dynamic_library.cpp
)
add_library( claw_dynamic_library SHARED ${CLAW_DYNAMIC_LIBRARY_SOURCES} )
add_library(
  claw_dynamic_library-static STATIC
  STATIC ${CLAW_DYNAMIC_LIBRARY_SOURCES}
)
target_link_libraries( claw_dynamic_library ${CMAKE_DL_LIBS} )

# log system
set( CLAW_LOGGER_SOURCES
  ${CLAW_CODE_DIR}android_logger.cpp
  ${CLAW_CODE_DIR}logger.cpp
  ${CLAW_CODE_DIR}log_stream.cpp
  ${CLAW_CODE_DIR}log_stream_concise.cpp
  ${CLAW_CODE_DIR}log_stream_uniq.cpp
  ${CLAW_CODE_DIR}log_level.cpp
)
add_library( claw_logger SHARED ${CLAW_LOGGER_SOURCES} )
add_library( claw_logger-static STATIC ${CLAW_LOGGER_SOURCES} )

target_link_libraries(
  claw_logger ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY}
  )

find_library( INTL_LIBRARY intl )

if ( INTL_LIBRARY )
  target_link_libraries(
    claw_logger ${INTL_LIBRARY}
    )
endif( INTL_LIBRARY )

if( WIN32 )
  set_target_properties( claw_logger-static
    PROPERTIES COMPILE_FLAGS
    "-DCLAW_LOGGER_NO_EXPORT" )
endif( WIN32 )

# network
set( CLAW_NET_SOURCES
  ${CLAW_CODE_DIR}basic_socket.cpp
  ${CLAW_CODE_DIR}socket_server.cpp
)
add_library( claw_net SHARED ${CLAW_NET_SOURCES} )
add_library( claw_net-static STATIC ${CLAW_NET_SOURCES} )

if( WIN32 )
  target_link_libraries( claw_net wsock32 )
endif( WIN32 )

# graphic
set( CLAW_GRAPHIC_LIBRARIES )
set( CLAW_GRAPHIC_SOURCES
     ${CLAW_CODE_DIR}image.cpp
     ${CLAW_CODE_DIR}bitmap.cpp
     ${CLAW_CODE_DIR}bitmap_writer.cpp
     ${CLAW_CODE_DIR}bitmap_reader.cpp
     ${CLAW_CODE_DIR}gif.cpp
     ${CLAW_CODE_DIR}gif_frame.cpp
     ${CLAW_CODE_DIR}gif_reader.cpp
     ${CLAW_CODE_DIR}pcx.cpp
     ${CLAW_CODE_DIR}pcx_reader.cpp
     ${CLAW_CODE_DIR}pcx_writer.cpp
     ${CLAW_CODE_DIR}pixel.cpp
     ${CLAW_CODE_DIR}targa.cpp
     ${CLAW_CODE_DIR}targa_writer.cpp
     ${CLAW_CODE_DIR}targa_reader.cpp
     ${CLAW_CODE_DIR}targa_file_structure.cpp
     ${CLAW_CODE_DIR}xbm.cpp
     ${CLAW_CODE_DIR}xbm_reader.cpp
     ${CLAW_CODE_DIR}xbm_writer.cpp
 )

# check jpeg library
if( JPEG_FOUND )
  set( CLAW_GRAPHIC_LIBRARIES ${CLAW_GRAPHIC_LIBRARIES} ${JPEG_LIBRARIES} )
  set( CLAW_GRAPHIC_SOURCES ${CLAW_GRAPHIC_SOURCES}
      ${CLAW_CODE_DIR}jpeg.cpp
      ${CLAW_CODE_DIR}jpeg_reader.cpp
      ${CLAW_CODE_DIR}jpeg_writer.cpp
      ${CLAW_CODE_DIR}jpeg_error_manager.cpp
   )
else( JPEG_FOUND )
  message( "jpeg library not found." )
endif( JPEG_FOUND )

# check png library
if( PNG_FOUND )
  set( CLAW_GRAPHIC_LIBRARIES ${CLAW_GRAPHIC_LIBRARIES} ${PNG_LIBRARIES} )
  set( CLAW_GRAPHIC_SOURCES ${CLAW_GRAPHIC_SOURCES}
      ${CLAW_CODE_DIR}png.cpp
      ${CLAW_CODE_DIR}png_reader.cpp
      ${CLAW_CODE_DIR}png_writer.cpp
   )
else( PNG_FOUND )
  message( "png library not found." )
endif( PNG_FOUND )

add_library( claw_graphic SHARED ${CLAW_GRAPHIC_SOURCES} )
add_library( claw_graphic-static STATIC ${CLAW_GRAPHIC_SOURCES} )
target_link_libraries(claw_graphic ${CLAW_GRAPHIC_LIBRARIES})

# tween
set( CLAW_TWEEN_SOURCES
  ${CLAW_CODE_DIR}tween/base_tweener.cpp
  ${CLAW_CODE_DIR}tween/single_tweener.cpp
  ${CLAW_CODE_DIR}tween/tweener.cpp
  ${CLAW_CODE_DIR}tween/tweener_group.cpp
  ${CLAW_CODE_DIR}tween/tweener_sequence.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_circ.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_none.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_linear.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_elastic.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_quint.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_cubic.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_back.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_quad.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_sine.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_quart.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_bounce.cpp
  ${CLAW_CODE_DIR}tween/easing/easing_expo.cpp
)

add_library( claw_tween SHARED ${CLAW_TWEEN_SOURCES} )
add_library(
  claw_tween-static
  STATIC ${CLAW_TWEEN_SOURCES}
)

IF(CMAKE_COMPILER_IS_GNUCXX)
  # extra flags for the link
  set_target_properties(
     ${CLAW_LIBRARIES}
     PROPERTIES LINK_FLAGS "-s -Wl,--as-needed" )
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

# all libraries
set( CLAW_ALL_LIBRARIES ${CLAW_LIBRARIES} ${CLAW_GRAPHIC_LIBRARIES})

#-------------------------------------------------------------------------------
# subdirectories
subdirs( doc claw desktop )

#-------------------------------------------------------------------------------
# install libraries
foreach( lib ${CLAW_LIBRARIES} )
  set_target_properties(
    ${lib} PROPERTIES
    VERSION ${CLAW_VERSION}
    SOVERSION ${CLAW_VERSION_MAJOR}
  )

  set_target_properties(
    ${lib}-static
    PROPERTIES OUTPUT_NAME
    ${lib}
    CLEAN_DIRECT_OUTPUT 1
    )

  install( TARGETS ${lib} DESTINATION ${CLAW_INSTALLDIR_LIB}  )
  install( TARGETS ${lib}-static DESTINATION ${CLAW_INSTALLDIR_LIB}  )
endforeach( lib )

install( FILES COPYING DESTINATION ${CLAW_INSTALLDIR_DOC} )

# target to uninstall files
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

if(UNIX)
  # configuration script
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/claw-config.in"
    "${CMAKE_CURRENT_BINARY_DIR}/claw-config"
    IMMEDIATE @ONLY)

  install( FILES "${CMAKE_CURRENT_BINARY_DIR}/claw-config"
    DESTINATION ${CLAW_EXECUTABLE_DIR}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE )
endif(UNIX)

# version
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/claw/version.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/claw/version.hpp"
)

install( FILES
  "${CMAKE_CURRENT_BINARY_DIR}/claw/version.hpp"
  DESTINATION "${CLAW_INSTALLDIR_SOURCE}" )

if(WIN32)
  subdirs(win)
endif(WIN32)

# check if we have to install cmake modules
if( CLAW_INSTALL_CMAKE_MODULES )
  subdirs( cmake-module )
endif( CLAW_INSTALL_CMAKE_MODULES )
