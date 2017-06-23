You'll need to have installed on your system:
- igraph (C package)
- OpenCV (C++ headers & python3 package)
- Matplotlib (python3)
- NetworkX (python3)
- Numpy (python3)


Compile:
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Usage:

	$ ./image_to_graph <image name> <neighbourhood radius> <pixel similarity constant> [reduce: 1..3 (default: 1)]
