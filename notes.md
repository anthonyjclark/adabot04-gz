
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
