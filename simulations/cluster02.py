#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
A virialized cluster.
Created on Wed Nov 15 15:48:44 2017

@author: chongchonghe
"""

from __future__ import print_function, division
import numpy as np
import matplotlib.pyplot as plt
from astropy.units import M_sun
from astropy.constants import M_sun, G

def init(N, semix, semiy, semiz):
    # Generate uniformly distributed particles inside a sphere, then stretched into an ellipsoid.
    # x^2/a^2 + y^2/b^2 + z^2/c^2 = 1
    N = 1000
    semiaxis = [semix, semiy, semiz]
    x, y, z = [np.random.random(3 * N) * 2 * semiax - semiax for semiax in semiaxis]
    isInside = x**2/semiaxis[0]**2 + y**2/semiaxis[1]**2 + z**2/semiaxis[2]**2 <= 1
    assert(np.sum(isInside) > N)
    x = x[isInside][:N]
    y = y[isInside][:N]
    z = z[isInside][:N]
    m = np.random.random(N)
    data = np.zeros([4, N])
    for i, xx in zip(range(4), [m, x, y, z]):
        data[i, :] = xx
    data = np.transpose(data)
    return data

def set_vel(data, alpha, radius):
    """ Set virialized velocities of a cluster """
    
#    alpha = 0.4
    mTot = np.sum(data[:, 0])
    vel_sq = alpha * mTot / radius
    velx = np.sqrt(vel_sq / 3)
    print("sigma_x =", velx)
    print("velocity dispersion crossing time:", radius / velx)
    N = len(data)
    vel = np.random.randn(N, 3) * velx
    return vel
    
    
N = 1000
positioins = init(N, 1, 1, 1)
alpha = 0.4
radius = 1

cluster = np.zeros([N, 7])
cluster[:, :4] = positioins
cluster[:, 4:] = set_vel(positioins, alpha, radius)

np.savetxt("cluster02.txt", cluster)


#m = cluster[:, 0]
#x = cluster[:, 1]
