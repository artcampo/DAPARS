#!/bin/sh

rm -rf build
rm -rf 
mkdir build
cd build
cmake ../../ -DCMAKE_INSTALL_PREFIX=../../GrammarAnalysis/install
make -j4 && make install -j4
