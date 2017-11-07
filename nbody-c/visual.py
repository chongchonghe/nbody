#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov  6 17:09:22 2017

@author: chongchonghe
"""
from __future__ import division, print_function
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
from mayavi import mlab

x, y, z = np.random.random([3, 1000])

plt.close('all')
fig = plt.figure()
#ax.set_axes('off')
ax = Axes3D(fig)
ax.scatter(x, y, z)
ax.set_xticks([])
ax.set_yticks([])
ax.set_zticks([])

x, y, z, value = np.random.random((4, 40))
mlab.points3d(x, y, z, value)