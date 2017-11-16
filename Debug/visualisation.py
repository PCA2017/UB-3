#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt

x = [100,500,1000]

fig = plt.figure(1)
# red dashes, blue squares and green triangles
plt.plot(x, [0.024495,0.662954,2.59087], marker='o', label="No Flag") 
plt.plot(x, [0.006763,0.200372,1.03645], marker='o', label="-O1") 
plt.plot(x, [0.004138,0.150887,0.932563], marker='o', label="-O2")
plt.plot(x, [0.003777,0.123444,0.919122], marker='o', label="-O3")

plt.title('Laufzeit auf peac_gcc')
plt.xlabel('Dimension der quadratischen Matrix n')
plt.ylabel('Laufzeit in Sekunden')
plt.legend(loc='upper left')

fig.savefig('Laufzeit_heated_plate_peac_gcc.png', dpi = 300)

plt.show()
