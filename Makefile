all:
	g++ image_to_graph.cc -o image_to_graph -O3 -Wall `pkg-config --cflags --libs opencv igraph`
	@#echo "Build the dependencies, the local program and then run some tests:"
	@#echo "  make deps"
	@#echo "  make build"
	@#echo "  make tests"

#.PHONY: deps build tests clean cleandeps
.PHONY: run tests clean

#deps:
	#@cd igraph && autoreconf -f -i && ./configure && cd ..
	#$(MAKE) -j $(nproc) -C igraph

#build:
	#g++ image_to_graph.cc -o image_to_graph -O3 -Wall -I ./igraph/include -L ./igraph/src/.libs -ligraph `pkg-config --cflags --libs opencv igraph`

run:
	./image_to_graph assets/lena.jpg

tests:
	@echo "Test: Lena"
	./image_to_graph assets/lena.jpg && python display_graph.py assets/lena.jpg
	@echo "Test: Arara"
	./image_to_graph assets/arara.jpg && python display_graph.py assets/arara.jpg
	@echo "Test: Shapes"
	./image_to_graph assets/shapes.png && python display_graph.py assets/shapes.png
	@echo "Test: Jacobite"
	./image_to_graph assets/jacobite.jpg && python display_graph.py assets/jacobite.jpg

clean:
	rm -f image_to_graph

#cleandeps:
	#$(MAKE) -C igraph clean
