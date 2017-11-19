#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""

@author: chongchonghe
"""

from __future__ import division, print_function
import numpy as np
import matplotlib.pyplot as plt
import glob
import time
import sys

#if len(sys.argv) != 4:
#    raise SystemExit("usage: ./movie2d.py path/to/data/dir boxsize pausetime")

fig, ax = plt.subplots(2, figsize=[6, 12])
#plt.show()

if len(sys.argv) >= 4:
    boxlen = np.double(sys.argv[3])
    xlim = [-boxlen, boxlen]
    ylim = [-boxlen, boxlen]
if len(sys.argv) == 5:
    startid = int(sys.argv[4])
else:
    startid = 0

#elif len(sys.argv) == 7:
#    xlim = [np.double(sys.argv[3]), np.double(sys.argv[4])]
#    ylim = [np.double(sys.argv[5]), np.double(sys.argv[6])]
#else:
#    raise SystemExit("usage: ./movie2d.py file pausetime [[boxlen], [xmin, xmax, ymin, ymax]]")

pausetime = np.double(sys.argv[2])

idx = 0
files = sorted(glob.glob(sys.argv[1] + "/*"))[startid:]
for fn in files:
    data = np.loadtxt(fn)
    x = data[:, 1]
    y = data[:, 2]
    ax[0].cla()
    ax[0].set_xlim(xlim)
    ax[0].set_ylim(ylim)
    #ax[0].plot(x[:-1], y[:-1], '.')
    #ax[0].plot(x[-1], y[-1], '.')
    ax[0].plot(x[0], y[0], '.')
    ax[0].plot(x[1], y[1], '.')
    ax[0].plot(x[2], y[2], '.')
    #plt.draw()

    # plot distance of last two particles in log scale
    ax[1].set_yscale('log')
    ax[1].set_ylim([1e-3, 1e3])
    ax[1].set_xlim([0, 404])
    ax[1].plot(idx, np.linalg.norm(data[-2, 1:4] - data[-1, 1:4]),'r.')
    idx += 1
    plt.pause(pausetime)


