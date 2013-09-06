#!/bin/sh

CXX=$ANDROID_CXX cmake . \
    -DCMAKE_INSTALL_PREFIX=$ANDROID_TOOLCHAIN_ROOT/sysroot/usr \
    -DCMAKE_BUILD_TYPE=release

