#!/bin/bash

mkdir build-cmake   || exit 1
cd build-cmake      || exit 1
cmake ..            || exit 1
make -j3            || exit 1
./test_vcurl        || exit 1 
cd ..               || exit 1
rm -r build-cmake   || exit 1

mkdir build-qmake   || exit 1
cd build-qmake      || exit 1
qmake ..            || exit 1
make -j3            || exit 1
./test_vcurl        || exit 1 
cd ..               || exit 1
rm -r build-qmake   || exit 1

exit 0;



