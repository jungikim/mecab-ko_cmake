#! /bin/bash

set -e
set -x

ROOT_DIR=$PWD

rm -rf build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_ROOT ..
cmake --build . --config Release --target install
