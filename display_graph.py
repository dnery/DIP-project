print('Importing shit... ', end='', flush=True)

import matplotlib.pyplot as pl
import networkx as nx
import numpy as np

print('Done.')
##############


## Build graph
print('Rebuilding graph... ', end='', flush=True)

with open('image_to_graph.out', 'rb') as input:
    dims = next(input, '').split()
    rows = int(dims[0])
    cols = int(dims[1])
    G = nx.read_edgelist(input, nodetype=int)

print('Done.')

print('Making layout... ', end='', flush=True)

layout = {}
for row in range(rows):
    for col in range(cols):
        layout[row*cols+col] = [col,rows-row]

print('Done.')

## Check results
nx.draw_networkx_edges(G, pos=layout)
pl.show()
