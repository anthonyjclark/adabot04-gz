#!/usr/bin/env bash

# Setup environment
if [[ ":$GAZEBO_MODEL_PATH" == *":$(pwd)"* ]]; then

    if [[ "$1" == "heightmap" ]]; then
        gazebo --pause --verbose worlds/test_heightmap.world
    else
        gazebo --pause --verbose worlds/empty.world
    fi

else
    echo "You need to source setup_env.sh"
fi
