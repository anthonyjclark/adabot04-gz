#!/usr/bin/env bash

# Setup environment
if [[ ":$GAZEBO_MODEL_PATH" == *":$(pwd)"* ]]; then

    if [[ "$1" == "hm-rgb" ]]; then
        gazebo --pause --verbose worlds/heightmap-rgb.world
    else if [[ "$1" == "hm-dirt" ]]; then
        gazebo --pause --verbose worlds/heightmap-dirt.world
    else
        gazebo --pause --verbose worlds/empty.world
    fi

else
    echo "You need to source setup_env.sh"
fi
