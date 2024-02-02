#! /bin/bash

set -e
set -x

ROOT_DIR=$PWD

pip install "cmake==3.18.*"

rm -rf build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_ROOT} -DCMAKE_BUILD_TYPE=Release ..
VERBOSE=1 make install
cd $ROOT_DIR

