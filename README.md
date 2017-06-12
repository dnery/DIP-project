You'll need to have installed on your system:
- automake/autoconf/autoreconf (Linux package)
- OpenCV (opencv2 headers & python3 package)
- Matplotlib (python3)
- NetworkX (python3)
- Numpy (python3)


The `igraph` C library is also used and bundled together. It takes a few minutes
to build (`make deps` step), so just hang in there.


Compile:
```
make deps
make build
```


Run tests:
```
make tests
```
