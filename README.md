
# Quickstart

```
mkdir -p ~/projects
cd ~/projects
git clone https://github.com/anthonyjclark/adabot04-gz.git
cd adabot04-gz/controller
./build.sh
cd ..
. setup_env.bash
./run.sh
```

The simulation will start `paused`. You will need to push play for anything to happen. You might also notice some warning (yellow text) and errors (red text). These are normal and they have to do with some outdated libraries in Gazebo.

To view the camera output:

- Open up the topic viewer with `Ctrl-T`
- Click on `gazebo.msgs.Imagestamped->/gazebo/default/ugv/chassis/stereo_camera/images`

# TODO

- Create link for camera and tilt it towards the ground
- Add wheel extensions
- Move model plugin to model/ugv directory (after `erb`)
- Convert camera output images to video ([tutorial](http://gazebosim.org/tutorials?tut=camera_save&cat=sensors#ConvertImagestoVideo))
- Running the simulation in headless mode ([tutorial](http://answers.gazebosim.org/question/14625/running-a-camera-sensor-headless/))

# Saving images

To save the stereo camera images you must change two lines in `models/ugv/model.sdf`. Find the following lines:

~~~xml
<save enabled="false">
~~~

and change `false` to `true`.

Your images will be saved in `/tmp/stereo_camera_left` and `/tmp/stereo_camera_right`. You can watch these directores with the following command: `watch ls -l /tmp/stereo_camera_left`.

# Running a world

Before you can run Gazebo you need to tell it where to find the controller plugin and the model files. You can do so with this command:

~~~bash
. setup_env.bash
~~~

Next, to run the a world you can type:

~~~bash
./run.sh 
# or ./run.sh heightmap
~~~

The `run.sh` script is just a quick wrapper around the following commands:

~~~bash
gazebo --pause --verbose worlds/empty.world
gazebo --pause --verbose worlds/test_heightmap.world
~~~

# Building the controller plugin

~~~bash
cd ~/projects/adabot04-gz/controller

# Go to directory and run
./bulid.sh
~~~

# Updating terrain model

Gazebo caches the heightmap model. So, after making changes to `models/terrain/model.sdf` you must clear the cache with the following command:

~~~bash
rm -r ~/.gazebo/paging/terrain-heightmap/
~~~

# Creating new terrains

The terrain model directory (`./models/terrain`) contains the two files needed to create a heightmap-style terrain in Gazebo. `terrain-heighmap.png` is a grayscale image where the intensity of a pixel indicates the height, and `terrain-texture.png` is a texture that can be used to *label* the different terrain types. See `notebooks/create_texture.ipynb` for an example of how to create a texture (you an use similar techniques for creating the heightmap image).

# Generating SDF from erb

You can use an "embedded ruby template file" to create SDF models (see [this example](https://bitbucket.org/osrf/gazebo_tutorials/raw/kinematic_loop/kinematic_loop/four_bar_sdf/model.sdf.erb)).

~~~bash
# Run with default parameters
erb model.sdf.erb > model.sdf

# Run with set parameters
erb var1=3 var2=4 model.sdf.erb > model.sdf
~~~

# From Brew after `brew install gazebo10`

(This is a note to myself. I was having issues with gazebo10 on macos.)

==> numpy
If you use system python (that comes - depending on the OS X version -
with older versions of numpy, scipy and matplotlib), you may need to
ensure that the brewed packages come earlier in Python's sys.path with:
  mkdir -p /Users/ajc/Library/Python/2.7/lib/python/site-packages
  echo 'import sys; sys.path.insert(1, "/usr/local/lib/python3.7/site-packages")' >> /Users/ajc/Library/Python/2.7/lib/python/site-packages/homebrew.pth


git -c user.name='Anthony Clark' -c user.email='anthonyjclark@gmail.com' commit -am 