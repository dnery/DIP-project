all:
	$(MAKE) -C igraph
	g++ image_to_graph.cc -o image_to_graph -Wall -ligraph `pkg-config --cflags --libs opencv`

config:
	cd igraph && ./configure && cd ..

.PHONY: clean allclean

clean:
	rm -f image_to_graph

allclean:
	$(MAKE) -C igraph clean
