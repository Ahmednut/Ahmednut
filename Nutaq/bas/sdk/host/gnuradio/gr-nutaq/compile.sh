#!/bin/sh
export PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig/

rm -rf build
mkdir build
cd build
cmake ..
make

