#!/bin/sh

rm -rf build
rm -rf 
mkdir build
cd build
cmake ../../ -DCMAKE_INSTALL_PREFIX=../../RecDescent/install
make -j4 && make install -j4
