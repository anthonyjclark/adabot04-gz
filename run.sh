#!/usr/bin/env bash

if [[ "$1" == "heightmap" ]]; then
    gazebo --pause --verbose worlds/test_heightmap.world
else
    gazebo --pause --verbose worlds/empty.world
fi
