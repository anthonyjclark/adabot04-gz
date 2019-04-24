#!/usr/bin/env bash

echo "This should be run from the script's directory"

if [ ! -d "build" ]; then
    mkdir build
    cd build
    cmake ..
    make
else
    echo "Calling make"
    echo "Delete \"build\" if you've made changes to CMakeLists.txt"
    cd build
    make
fi
