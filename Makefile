all:
	@echo "Build the dependencies, the local program and then run some tests:"
	@echo "  make deps"
	@echo "  make build"
	@echo "  make tests"

.PHONY: deps build tests clean cleandeps

deps:
	@cd igraph && autoreconf -f -i && ./configure && cd ..
	$(MAKE) -C igraph

build:
	g++ image_to_graph.cc -o image_to_graph -Wall -I ./igraph/include -L ./igraph/src/.libs -ligraph `pkg-config --cflags --libs opencv`

tests:
	@echo "Test: Lena"
	LD_LIBRARY_PATH=./igraph/src/.libs ./image_to_graph assets/lena.jpg && python display_graph.py assets/lena.jpg
	@echo "Test: Shapes"
	LD_LIBRARY_PATH=./igraph/src/.libs ./image_to_graph assets/shapes.png && python display_graph.py assets/shapes.png
	@echo "Test: Jacobite"
	LD_LIBRARY_PATH=./igraph/src/.libs ./image_to_graph assets/jacobite.jpg && python display_graph.py assets/jacobite.jpg
	@echo "Test: Arara"
	LD_LIBRARY_PATH=./igraph/src/.libs ./image_to_graph assets/arara.jpg && python display_graph.py assets/arara.jpg

clean:
	rm -f image_to_graph

cleandeps:
	$(MAKE) -C igraph clean
