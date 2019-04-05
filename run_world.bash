#!/usr/bin/env bash

export GAZEBO_PLUGIN_PATH=$(pwd)/lib:$GAZEBO_PLUGIN_PATH
export GAZEBO_MODEL_PATH=$(pwd)/models:$GAZEBO_MODEL_PATH
export GAZEBO_RESOURCE_PATH=$(pwd)/models:$GAZEBO_RESOURCE_PATH

echo GAZEBO_PLUGIN_PATH is $GAZEBO_PLUGIN_PATH
echo GAZEBO_MODEL_PATH is $GAZEBO_MODEL_PATH
echo GAZEBO_RESOURCE_PATH is $GAZEBO_RESOURCE_PATH

gazebo --pause --verbose worlds/empty.world
