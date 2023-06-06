#!/bin/bash

# Show executed shell commands
set -o xtrace
# Exit on error.
set -e

rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake -DPLATFORM=OS64 -DDEPLOYMENT_TARGET=13.0
make -j
make install