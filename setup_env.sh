#
# Source this file to export necessary environment variables
# run with
#    source setup_env.bash
#

# Path to the controller plugin library (must be built first)
export GAZEBO_PLUGIN_PATH=$(pwd)/plugins/keyboard_controller/build:$GAZEBO_PLUGIN_PATH

# Path to the GUI plugin library (must be built first)
export GAZEBO_PLUGIN_PATH=$(pwd)/plugins/keyboard_gui/build:$GAZEBO_PLUGIN_PATH

export GAZEBO_PLUGIN_PATH=$(pwd)/plugins/fsm_controller/build:$GAZEBO_PLUGIN_PATH

# All models are in the models directory
export GAZEBO_MODEL_PATH=$(pwd)/models:$GAZEBO_MODEL_PATH

# The following export seems to break Gazebo
# export GAZEBO_RESOURCE_PATH=$(pwd)/models:$GAZEBO_RESOURCE_PATH

echo "GAZEBO_PLUGIN_PATH   " is $GAZEBO_PLUGIN_PATH
echo "GAZEBO_MODEL_PATH    " is $GAZEBO_MODEL_PATH
echo "GAZEBO_RESOURCE_PATH " is $GAZEBO_RESOURCE_PATH
