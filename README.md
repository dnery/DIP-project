Compile:
`g++ image_to_graph.cc -o image_to_graph -Wall -ligraph -fopenmp ``pkg-config --cflags --libs opencv`` `

Run stack:
`./image_to_graph lena.jpg && python display_graph.py`
