#!/bin/bash

# Show executed shell commands
set -o xtrace
# Exit on error.
set -e

rm -rf ./vendor
mkdir -p vendor/include
mkdir -p vendor/lib
mkdir -p build

(
    rm -rf ./build/googletest
    git clone -b release-1.8.0 https://github.com/google/googletest.git build/googletest
    cd build/googletest
    cmake CMakeLists.txt -DCMAKE_TOOLCHAIN_FILE=../../ios.toolchain.cmake -DPLATFORM=OS64 -DCMAKE_INSTALL_PREFIX=../../vendor
    make -j
    make install
)

(
    rm -rf ./build/gflags
    git clone -b v2.2.1 https://github.com/gflags/gflags.git build/gflags
    cd build/gflags
    cmake CMakeLists.txt -DCMAKE_TOOLCHAIN_FILE=../../ios.toolchain.cmake -DPLATFORM=OS64 -DCMAKE_INSTALL_PREFIX=../../vendor
    make -j
    make install
)

(
    rm -rf ./build/glog
    git clone -b 0.3.5-ios https://github.com/reilabs/glog.git build/glog
    cd build/glog
    cmake CMakeLists.txt -DCMAKE_TOOLCHAIN_FILE=../../ios.toolchain.cmake -DPLATFORM=OS64 -DCMAKE_INSTALL_PREFIX=../../vendor
    make -j
    make install
)

(
    rm -rf ./build/benchmark
    git clone -b v1.4.0 https://github.com/google/benchmark.git build/benchmark
    cd build/benchmark
    cmake CMakeLists.txt -DCMAKE_TOOLCHAIN_FILE=../../ios.toolchain.cmake -DPLATFORM=OS64 -DCMAKE_INSTALL_PREFIX=../../vendor # -DCMAKE_BUILD_TYPE=Release 
    make -j
    make install
)
