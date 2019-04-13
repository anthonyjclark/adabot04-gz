#!/usr/bin/env bash

erb -T 2 model.sdf.erb > model.sdf
sed -i '' '/^[[:space:]]*$/d' model.sdf
