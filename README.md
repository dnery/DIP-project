You'll need to have installed on your system:
- OpenCV (C++ headers & python3 package)
- igraph (C package)

Compile:
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Usage:

	$ ./image_to_graph <image name> <neighbourhood radius> <pixel similarity constant> [reduce: 1..3 (default: 1)]

The `docs` folder contains the project documentation, including the
presentation, experiment analysis and final considerations.
