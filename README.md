

TODO:

- Create link for camera and tilt it towards the ground
- Create textures for the labeling the terrain

git -c user.name='Anthony Clark' -c user.email='anthonyjclark@gmail.com' commit -am ""

http://gazebosim.org/tutorials?tut=camera_save&cat=sensors#ConvertImagestoVideo
http://answers.gazebosim.org/question/14625/running-a-camera-sensor-headless/


cd projects/adabot04-gz/
. setup_env.bash 
./run.sh

starts paused

Window->Topic Visualization (or ctrl-T)
gazebo.msgs.Imagestamped->/gazebo/default/ugv/chassis/stereo_camera/images

    <save enabled="false">

watch ls -l stereo_camera_left


# Running a world

`. setup_env.bash`

`gazebo --pause --verbose worlds/empty.world`

`gazebo --verbose worlds/test_heightmap.world`

# Building and running

The following are just helper scripts. They are not necessary, but they serve as documentation.

```bash
# Go to directory and run
./bulid.sh

# Run the following if you make changes
./make.sh

# Running an executable
./run.sh
```

# Updating terrain model

Need to clear cache
~/.gazebo/paging/terrain-heightmap/

# From Brew after `brew install gazebo10`

==> numpy
If you use system python (that comes - depending on the OS X version -
with older versions of numpy, scipy and matplotlib), you may need to
ensure that the brewed packages come earlier in Python's sys.path with:
  mkdir -p /Users/ajc/Library/Python/2.7/lib/python/site-packages
  echo 'import sys; sys.path.insert(1, "/usr/local/lib/python3.7/site-packages")' >> /Users/ajc/Library/Python/2.7/lib/python/site-packages/homebrew.pth
