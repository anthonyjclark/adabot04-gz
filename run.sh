#!/usr/bin/env bash

# Check environment variables
if [[ ":$GAZEBO_MODEL_PATH" != *":$(pwd)"* ]]; then
    echo "You need to source setup_env.sh"
    exit
fi

# Check for plugins
plugins_counter=$((0))
for path in ${GAZEBO_PLUGIN_PATH//:/ }; do
    if [ -f "$path""/libkeyboard_controller.so" ]; then
        ((plugins_counter++))
    fi
    if [ -f "$path""/libKeyboardGUIPlugin-FIXED.so" ]; then
        ((plugins_counter++))
    fi
done

echo

if [[ "$plugins_counter" < 2 ]]; then
    echo "You needo build the plugins"
    exit
fi

# Run the simulation
if [[ "$1" == "hm-rgb" ]]; then
    gazebo --pause --verbose worlds/heightmap-rgb.world
elif [[ "$1" == "hm-dirt" ]]; then
    gazebo --pause --verbose worlds/heightmap-dirt-high.world
else
    gazebo --pause --verbose worlds/empty.world
fi
