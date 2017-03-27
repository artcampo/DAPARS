#!/bin/sh

rm -rf build
rm -rf install
mkdir build
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=../install
make -j4 && make install -j4
