#!/usr/bin/env bash

cd plugins/keyboard_controller/
./build.sh

cd ../fsm_controller/
./build.sh

cd ../keyboard_gui/
./build.sh