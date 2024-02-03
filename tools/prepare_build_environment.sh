#! /bin/bash

set -e
set -x

ROOT_DIR=$PWD
CMAKE_EXTRA_ARGS=""

if [ "$CIBW_ARCHS" == "arm64" ]; then
    CMAKE_EXTRA_ARGS="-DCMAKE_OSX_ARCHITECTURES=arm64"
fi

pip install "cmake==3.28.1"

rm -rf build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_ROOT} -DCMAKE_BUILD_TYPE=Release $CMAKE_EXTRA_ARGS ..
VERBOSE=1 make install
cd $ROOT_DIR

