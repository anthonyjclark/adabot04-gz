#!/usr/bin/env bash

echo "Run from script's directory"

mkdir build
cd build
cmake ..
make
