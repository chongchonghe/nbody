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

plt.figure()
#plt.show()

boxlen = np.double(sys.argv[2])

for fn in glob.glob(sys.argv[1] + "/*"):
    data = np.loadtxt(fn)
    x = data[:, 1]
    y = data[:, 2]
    plt.clf()
    plt.xlim([-boxlen, boxlen])
    plt.ylim([-boxlen, boxlen])
    plt.plot(x[:-1], y[:-1], '.')
    plt.plot(x[-1], y[-1], 'r.')
    #plt.draw()
    plt.pause(0.1)
