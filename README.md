You'll need OpenCV for this. The igraph package is also needed and it's included.

Compile:
```
make config
make
```

Run stack:
```
./image_to_graph <input_image> && python display_graph.py <input_image>
```
