print('Importing shit... ', end='', flush=True)

import matplotlib.pyplot as pl
import networkx as nx
import numpy as np

print('Done.')
##############


## Build graph
print('Rebuilding graph... ', end='', flush=True)

with open('image_as_graph.txt', 'rb') as input:
    next(input, '')
    G = nx.read_edgelist(input, nodetype=int)

print('Done.')

## Check results
nx.draw_networkx_edges(G, pos=nx.spring_layout(G))
pl.show()
