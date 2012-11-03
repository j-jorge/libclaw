#!/bin/sh

# MinGW must be in your path. Edit the registry key
# HKCU/Environment/PATH by typing
#
#  wine regedit
#

[ -z "$WIN_PROGRAM_FILES" ] \
    && WIN_PROGRAM_FILES="C:/Program Files"

[ -z "$WIN_GNU_INCLUDE_DIR" ] \
    && WIN_GNU_INCLUDE_DIR="$WIN_PROGRAM_FILES/GnuWin32/include"

[ -z "$WIN_GNU_LIB_DIR" ] \
    && WIN_GNU_LIB_DIR="$WIN_PROGRAM_FILES/GnuWin32/lib"

[ -z "$WIN_BOOST_DIR" ] \
    && WIN_BOOST_DIR="C:/boost_1_45_0"

# Libraries paths
[ -z "$WIN_JPEG_INCLUDE_DIR" ] \
    && WIN_JPEG_INCLUDE_DIR="$WIN_GNU_INCLUDE_DIR"

[ -z "$WIN_JPEG_LIBRARY_PATH" ] \
    && WIN_JPEG_LIBRARY_PATH="$WIN_GNU_LIB_DIR/libjpeg.dll.a"

[ -z "$WIN_PNG_INCLUDE_DIR" ] \
    && WIN_PNG_INCLUDE_DIR="$WIN_GNU_INCLUDE_DIR"

[ -z "$WIN_PNG_LIBRARY_PATH" ] \
    && WIN_PNG_LIBRARY_PATH="$WIN_GNU_LIB_DIR/libpng.dll.a"

[ -z "$WIN_BOOST_INCLUDE_DIR" ] \
    && WIN_BOOST_INCLUDE_DIR="$WIN_BOOST_DIR"

# Compilation flags
[ -z "$BUILD_TYPE" ] && BUILD_TYPE=release

[ -z "$INSTALL_PREFIX" ] && INSTALL_PREFIX=/tmp/install/win

# Compilation

rm -f CMakeCache.txt

wine C:/Program\ Files/CMake\ 2.8/bin/cmake.exe -G "MinGW Makefiles" . \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DJPEG_INCLUDE_DIR:PATH="$WIN_JPEG_INCLUDE_DIR" \
    -DJPEG_LIBRARY:FILEPATH="$WIN_JPEG_LIBRARY_PATH" \
    -DPNG_PNG_INCLUDE_DIR:PATH="$WIN_PNG_INCLUDE_DIR" \
    -DPNG_LIBRARY:FILEPATH="$WIN_PNG_LIBRARY_PATH" \
    -DBoost_INCLUDE_DIR:PATH="$WIN_BOOST_INCLUDE_DIR" \
    -DCMAKE_SHARED_LINKER_FLAGS='-Wl,--enable-auto-import,--enable-stdcall-fixup' \
    -DCMAKE_EXE_LINKER_FLAGS='-Wl,--enable-auto-import,--enable-stdcall-fixup' \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX"

wine mingw32-make -i

wine strip *.dll

cd doc
doxygen
cd ..

makensis "libclaw.nsi"
