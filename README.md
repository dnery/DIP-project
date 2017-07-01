You'll need to have installed on your system:
- OpenCV (C++ headers & python3 package)
- igraph (C package)
- CMake >= 2.6

Compile:
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Usage:
```sh
$ ./image_to_graph <image name> <pixel neighbourhood radius> <pixel similarity constant> [reduce: 1..3 (default: 1)]
```

The `docs` folder contains the project documentation, including the
presentation, experiment analysis and final considerations. The `run_tests.sh`
script will run the program on some of the input images in the `assets` folder
and output several segmentation results in the `results` folder. The experiment
result filenames are given as such:

`<input image name>.<pixel neighbourhodd radius>_<pixel similarity constant>_<OpenCV downscaling factor enum>.<input image format>`
