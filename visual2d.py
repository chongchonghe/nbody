#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""

@author: chongchonghe
"""

from __future__ import division, print_function
import numpy as np
import matplotlib.pyplot as plt
import glob
import sys

plt.figure()

for fn in glob.glob(sys.argv[1]+"/*"):
    data = np.loadtxt(fn)
    x = data[:, 1]
    y = data[:, 2]
    plt.clf()
    plt.plot(x, y, '.')
    plt.sleep(0.1)
