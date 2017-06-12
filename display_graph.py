print('Importing shit... ', end='', flush=True)

import matplotlib.pyplot as pl
import networkx as nx
import numpy as np
import sys
import cv2

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

## Build layout
print('Making layout... ', end='', flush=True)

layout = {}
for row in range(rows):
    for col in range(cols):
        layout[row*cols+col] = [col,rows-row]

nx.draw_networkx_edges(G, pos=layout)
pl.savefig('image_to_graph.out.png')

print('Done.')

## Display results
print('Displaying (\'q\' to quit)... ', end='', flush=True)

imga = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
imgb = cv2.imread('image_to_graph.out.png', cv2.IMREAD_GRAYSCALE)

ha,wa = imga.shape[:2]
hb,wb = imgb.shape[:2]

output = np.zeros((max(ha,hb),wa+wb), np.uint8) + 255
output[:ha,:wa]      = imga
output[:hb,wa:wa+wb] = imgb

cv2.imshow('Grayscale Image to Complex Network (\'q\' to quit)', output)

while(True):
    if cv2.waitKey(0) == ord('q'):
        break

cv2.destroyAllWindows()

print('Done.')
