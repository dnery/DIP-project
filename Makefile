exe = image_to_graph
libs = opencv igraph
CXXFLAGS = -O3 -Wall `pkg-config --cflags --libs $(libs)`

all: image_to_graph.o
	$(CXX) $^ -o $(exe) $(CXXFLAGS)

.PHONY: run tests clean

run:
	./$(exe) assets/lena.jpg 42 0.975f 2

tests:
	@echo "Test: Lena"
	./image_to_graph assets/lena.jpg 42 0.975f 2 && python display_graph.py assets/lena.jpg
	#@echo "Test: Arara"
	#./image_to_graph assets/arara.jpg && python display_graph.py assets/arara.jpg
	#@echo "Test: Shapes"
	#./image_to_graph assets/shapes.png && python display_graph.py assets/shapes.png
	#@echo "Test: Jacobite"
	#./image_to_graph assets/jacobite.jpg && python display_graph.py assets/jacobite.jpg

clean:
	rm -f image_to_graph
