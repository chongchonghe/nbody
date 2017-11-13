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
    """ Return the coordinates of particles given by the density profile
    rho(r) = rho(0) * exp(-r/scaleR) * exp(-h/scaleH) """

    r = ExpRandom(N, scaleR)
    H = ExpRandom(N, scaleH)
    theta = np.random.random(N) * 2 * np.pi
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    sides = np.random.random(N)
    sides = [-1 if side < 0.5 else 1 for side in sides]
    z = H * np.array(sides)
    return x, y, z


def galaxy(N1, N2, scaleR1, scaleH1, scaleR2, scaleH2, omega1, omega2):
    """ Return [m, x, y, z, vx, vy, vz] of a galaxy consisting of two disks with 
    the respective number of stars, scaleR, scaleH, and omega """
    
    galaxy = np.zeros([N1 + N2, 7])

    # thin disk
    mass1 = imf(N1)
    galaxy[:N1, 0] = mass1

    # coordinates
    x1, y1, z1 = disks(N1, scaleR1, scaleH1)
    galaxy[:N1, 1] = x1
    galaxy[:N1, 2] = y1
    galaxy[:N1, 3] = z1

    # velocity 
    # v = -omega y i + omega x j
    galaxy[:N1, 4] = -omega1 * galaxy[:N1, 2]
    galaxy[:N1, 5] = omega1 * galaxy[:N1, 1]

    # thick disk
    mass2 = imf(N2)
    galaxy[N1:, 0] = mass2

    # coordinates
    x2, y2, z2, = disks(N2, scaleR2, scaleH2)
    galaxy[N1:, 1] = x2
    galaxy[N1:, 2] = y2
    galaxy[N1:, 3] = z2

    # velocity 
    # v = -omega y i + omega x j
    galaxy[N1:, 4] = -omega2 * galaxy[N1:, 2]
    galaxy[N1:, 5] = omega2 * galaxy[N1:, 1]

    return galaxy


def rotate(arr, theta, phi, x, y, z):
    """ Rotate a N-by-3 vector from (0, 0) to (theat, phi) then translate to 
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


def main():

    ##### The Milky Way Galaxy: galaxy1 #####
    N1 = 1000
    scaleR1 = 3.5
    scaleH1 = 0.3
    N2 = int(N1 * 0.15)
    scaleR2 = 3.5
    scaleH2 = 1.0
    theta = 0
    phi = 0
    x = 0
    y = 30
    z = -10
    omega1 = 1
    omega2 = 1
    
    galaxy1 = galaxy(N1, N2, scaleR1, scaleH1, scaleR2, scaleH2, omega1, omega2)
    galaxy1[:, (1,2,3)] = rotate(galaxy1[:, (1,2,3)], theta, phi, x, y, z)
    galaxy1[:, (4,5,6)] = rotate(galaxy1[:, (4,5,6)], theta, phi, 0, 0, 0)

    ##### galaxy2 #####
    N1 = 1000
    scaleR1 = 3.5
    scaleH1 = 0.3
    N2 = int(N1 * 0.15)
    scaleR2 = 3.5
    scaleH2 = 1.0
    theta = np.pi / 6
    phi = np.pi / 4
    x = 0
    y = -30
    z = 10
    omega1 = 1
    omega2 = 1

    galaxy2 = galaxy(N1, N2, scaleR1, scaleH1, scaleR2, scaleH2, omega1, omega2)
    galaxy2[:, (1,2,3)] = rotate(galaxy2[:, (1,2,3)], theta, phi, x, y, z)
    galaxy2[:, (4,5,6)] = rotate(galaxy2[:, (4,5,6)], theta, phi, 0, 0, 0)

    data = np.concatenate((galaxy1, galaxy2))
    np.savetxt('disk/disk.txt', data)


if __name__ == "__main__":
    main()
