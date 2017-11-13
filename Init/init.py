#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
init.py
Produces the initial conditions for our simulations.
Created on Sun Nov 12 00:29:03 2017
@author: chongchonghe
"""

from __future__ import print_function, division
import numpy as np
import matplotlib.pyplot as plt
from kroupa import kroupa, imf
from morphology import rx, ExpRandom

def disks(N, scaleR, scaleH):
    """ rho(r) = rho(0) * exp(-r/scaleR) """

    r = ExpRandom(N, scaleR)
    H = ExpRandom(N, scaleH)
    theta = np.random.random(N) * 2 * np.pi
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    sides = np.random.random(N)
    sides = [-1 if side < 0.5 else 1 for side in sides]
    z = H * np.array(sides)
    return x, y, z

def rotate(arr, theta, phi, x, y, z):
    """ Rotate a N-by-3 vector from (0, 0) to (theat, phi) and translate to 
    (x, y, z). The rotation is equivalent to Ry(theta) then Rz(phi):
        arr' = arr * Ry(theta)^T * Rz(phi)^T
    """
    Rytheta = np.array([[np.cos(theta), 0, np.sin(theta)],
                        [0, 1, 0],
                        [-np.sin(theta), 0, np.cos(theta)]])
    Rzphi = np.array([[np.cos(phi), -np.sin(phi), 0],
                      [np.sin(phi), np.cos(phi), 0],
                      [0, 0, 1]])
    # rotation
    arr = np.dot(arr, np.transpose(Rytheta))
    arr = np.dot(arr, np.transpose(Rzphi))
    # transplation
    arr[:,0] += x
    arr[:,1] += y
    arr[:,2] += z
    return arr


def galaxy(N1, N2, scaleR1, scaleH1, scaleR2, scaleH2):

    # think disk
    mass1 = imf(N1)
    x1, y1, z1 = disks(N1, scaleR1, scaleH1)

    # thick disk
    mass2 = imf(N2)
    x2, y2, z2, = disks(N2, scaleR2, scaleH2)

    galaxy = np.zeros([N1 + N2, 4])
    galaxy[:N1, 0] = mass1
    galaxy[:N1, 1] = x1
    galaxy[:N1, 2] = y1
    galaxy[:N1, 3] = z1
    galaxy[N1:, 0] = mass2
    galaxy[N1:, 1] = x2
    galaxy[N1:, 2] = y2
    galaxy[N1:, 3] = z2

    return galaxy


##### The Milky Way Galaxy: galaxy1 #####
N1 = 1000
scaleR1 = 3.5
scaleH1 = 0.3
N2 = int(N1 * 0.15)
scaleR2 = 3.5
scaleH2 = 1.0
x = 0
y = 30
z = -10
galaxy1 = galaxy(N1, N2, scaleR1, scaleH1, scaleR2, scaleH2)
galaxy1[:, (1,2,3)] = rotate(galaxy1[:, (1,2,3)], 0, 0, x, y, z)

##### galaxy2 #####
N1 = 1000
scaleR1 = 3.5
scaleH1 = 0.3
N2 = int(N1 * 0.15)
scaleR2 = 3.5
scaleH2 = 1.0
galaxy2 = galaxy(N1, N2, scaleR1, scaleH1, scaleR2, scaleH2)
theta = np.pi / 6
phi = np.pi / 4
x = 0
y = -30
z = 10
galaxy2[:, (1,2,3)] = rotate(galaxy2[:, (1,2,3)], np.pi/6, 0, x, y, z)

data = np.concatenate((galaxy1, galaxy2))
np.savetxt('disk/disk.txt', data)
