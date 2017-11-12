#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
@author: M
"""
import sys
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation


def get_data (txtlink):
    f = open(txtlink)
    str_matrixes_list = f.read().split('#')
    str_matrixes_list.pop()                                                    # Letzte Matrix ist leer diese Entfernen
    
    matrix_list = []
    for m in str_matrixes_list:        
        tmp = m.split() 
        cols = len(tmp)                                                        # Dimension der Matrix speichern
        tmp = [float(x) for line in tmp for x in line.split(',') if x]         # Zeile mit den Werten der Matrix, nur Werte mit Zahlenwert als float
        tmp_np = np.array(tmp)                                                 # Matrix als Vektor laden
        matrix_list.append(np.reshape(tmp_np, (-1, cols)))                     # Matrix des Zeitschritts als 2D numpy Matrix anhängen.
            
    return matrix_list

# Import Data
textfile = 'file_1'
#textfile = sys.argv[1]                                                        # Aufruf z.B. "python3 visualisierung.py 470e"
matrix_list = get_data (textfile)

## Create Colormap
#cdict1 = {'red':   ((0.0, 0.0, 0.0),
#                   (0.5, 0.0, 0.1),
#                   (1.0, 1.0, 1.0)),
#
#         'green': ((0.0, 0.0, 0.0),
#                   (1.0, 0.0, 0.0)),
#
#         'blue':  ((0.0, 0.0, 1.0),
#                   (0.5, 0.1, 0.0),
#                   (1.0, 0.0, 0.0))
#        }
#
## Make a modified version of cdict1 with some transparency
## in the middle of the range.
#cdict2 = cdict1.copy()
#cdict2['alpha'] = ((0.0, 1.0, 1.0),
#                #   (0.25,1.0, 1.0),
#                   (0.5, 0.3, 0.3),
#                #   (0.75,1.0, 1.0),
#                   (1.0, 1.0, 1.0))
#        
#cm = matplotlib.colors.LinearSegmentedColormap('red2blue', cdict2, 1024)

# Create the first plot
fig, ax = plt.subplots()
matplot = ax.matshow(matrix_list[0], cmap='BuPu', vmin=0, vmax=100)
#plt.show()

# Update plot
def update_plot(frame, matrix_list, matplot):
    matplot.set_data(matrix_list[frame])
    return matplot

ani=animation.FuncAnimation(fig, update_plot, frames=len(matrix_list), fargs=(matrix_list, matplot))
#ani.save(sys.argv[1]+".gif", writer='imagemagick', fps=30);
ani.save("file_1.gif", writer='imagemagick', fps=30);
#plt.show()
#plt.savefig(sys.argv[1]+".png")
