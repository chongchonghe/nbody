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
import os

#N = 1000
mTot = 100
mMean = mTot / N
#alpha = 0.55 # mean particle mass
#m_tot = alpha * N * 1e9
#m_unit = Mtot / m_tot
t_unit = 14.91 # Myr
l_unit = 1 # kpc


isFlat = True


def disks(N, scaleR, scaleH):
    """ Return the coordinates of particles given by the density profile
    rho(r) = rho(0) * exp(-r/scaleR) * exp(-h/scaleH) """

#    r = ExpRandom(N, scaleR)
    r = np.random.random(N) * scaleR
    theta = np.random.random(N) * 2 * np.pi
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    if not isFlat:
        H = ExpRandom(N, scaleH)
        sides = np.random.random(N)
        sides = [-1 if side < 0.5 else 1 for side in sides]
        z = H * np.array(sides)
    else:
        z = 0
    
    return x, y, z

#    """ test: use a uniform cloud """
#    semiaxis = [scaleR] * 2
#    x, y = [np.random.random(3 * N) * 2 * semiax - semiax for semiax in semiaxis]
#    isInside = x**2/semiaxis[0]**2 + y**2/semiaxis[1]**2 <= 1
#    assert(np.sum(isInside) > N)
#    x = x[isInside][:N]
#    y = y[isInside][:N]
#    z = np.zeros(N)
#    data = np.zeros([N, 3])
#    for i, xx in zip(range(3), [x, y, z]):
#        data[:, i] = xx
#    return data[:, 0], data[:, 1], data[:, 2]


def galaxy(N1, N2, scaleR1, scaleH1, scaleR2, scaleH2): #, omega2):
    """ Return [m, x, y, z] of a galaxy consisting of two disks with 
    the respective number of stars, scaleR, scaleH, and omega """
    
    galaxy0 = np.zeros([N1 + N2, 7])

    # thin disk
#    mass1 = imf(N1) * m_unit
    mass1 = np.ones(N1) * mMean
    galaxy0[:N1, 0] = mass1

    # coordinates
    x1, y1, z1 = disks(N1, scaleR1, scaleH1)
    galaxy0[:N1, 1] = x1
    galaxy0[:N1, 2] = y1
    galaxy0[:N1, 3] = z1

#    # thick disk
##    mass2 = imf(N2) * m_unit
#    mass2 = np.ones(N2) * mMean
#    galaxy0[N1:, 0] = mass2
#
#    # coordinates
#    x2, y2, z2, = disks(N2, scaleR2, scaleH2)
#    galaxy0[N1:, 1] = x2
#    galaxy0[N1:, 2] = y2
#    galaxy0[N1:, 3] = z2

    return galaxy0


def centerofmass(m, r):
    mTot = np.sum(m)
    return [np.sum(m * r[:, i]) / mTot for i in range(3)]


def velocity(m, r):
    """ Assuming the center of mass is at the origin,
    calculate the velocity of each particle """
    
    n = len(m)
    radius = np.sqrt(r[:,0]**2 + r[:,1]**2 + r[:,2]**2)
#    plt.hist(radius)
    v = np.zeros(n)
    for i in range(n):
        mInside = np.sum(m[radius < radius[i]])
        if abs(radius[i]) < 1e-10:
            v[i] = 0
        else:
            v[i] = np.sqrt(mInside / radius[i])
    vel = np.zeros([n, 3])
    radius[radius<0.05] = np.inf
    vel[:, 0] = -v / radius * r[:, 1]
    vel[:, 1] = v / radius * r[:, 0]
    
    return vel


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
#    N2 = int(N1 * 0.15)
    N2 = 0
    scaleR2 = scaleR1
    scaleH2 = 1.0
  
    galaxy0 = galaxy(N1, N2, scaleR1, scaleH1, scaleR2, scaleH2)
    
    # move the center of mass to the origin
    xc, yc, zc = centerofmass(galaxy0[:, 0], galaxy0[:, (1, 2, 3)])
    for i, rc in zip(range(1, 4), [xc, yc, zc]):
        galaxy0[:, i] -= rc
        
#    galaxy0 = np.concatenate((galaxy0, np.array([[10000, 0, 0, 0, 0, 0, 0]])))
        
    m = np.array(galaxy0[:, 0])
    r = np.array(galaxy0[:, (1,2,3)])
    galaxy0[:, (4,5,6)] = velocity(m, r)
    
#    return galaxy0


#def rot(data, dt):
#    for i in range(3):
#        data[:, i + 1] += data[:, i + 4] * dt
#    r = np.sqrt(data[:, 1]**2 + data[:, 2]**2)
#    acce = (data[:, 4]**2 + data[:, 5]**2) / r
#    ax = -acce * data[:, 1] / r
#    ay = -acce * data[:, 2] / r
#    data[:, 4] += ax * dt
#    data[:, 5] += ay * dt
    

#                   
#    theta = 0
#    phi = 0
#    x = 0
#    y = 30
#    z = -10
#    
#    galaxy1 = np.array(galaxy0)
#    galaxy1[:, (1,2,3)] = rotate(galaxy1[:, (1,2,3)], theta, phi, x, y, z)
#    galaxy1[:, (4,5,6)] = rotate(galaxy1[:, (4,5,6)], theta, phi, 0, 0, 0)
#    
#    theta = np.pi / 6
#    phi = np.pi / 4
#    x = 0
#    y = -30
#    z = 10
#    
#    galaxy2 = np.array(galaxy0)
#    galaxy2[:, (1,2,3)] = rotate(galaxy2[:, (1,2,3)], theta, phi, x, y, z)
#    galaxy2[:, (4,5,6)] = rotate(galaxy2[:, (4,5,6)], theta, phi, 0, 0, 0)

    
#    ##### galaxy2 #####
#    N1 = 1000
#    scaleR1 = 3.5
#    scaleH1 = 0.3
#    N2 = int(N1 * 0.15)
#    scaleR2 = scaleR2
#    scaleH2 = 1.0
#    theta = np.pi / 6
#    phi = np.pi / 4
#    x = 0
#    y = -30
#    z = 10
##    omega1 = 1
##    omega2 = 1
#
#    galaxy2 = galaxy(N1, N2, scaleR1, scaleH1, scaleR2, scaleH2, omega1, omega2)
#    galaxy2[:, (1,2,3)] = rotate(galaxy2[:, (1,2,3)], theta, phi, x, y, z)
#    galaxy2[:, (4,5,6)] = rotate(galaxy2[:, (4,5,6)], theta, phi, 0, 0, 0)


    # data = np.concatenate((galaxy1, galaxy2))
    # np.savetxt('disk/disk.txt', data)


    np.savetxt('disk/disk.txt', galaxy0)
    return galaxy0


if __name__ == "__main__":
    
    g = main()g
    
#    data = main()
#    freq = 4000
#    ntot = 100000
#    for i in range(ntot):
#        rot(data, 0.00005)
#        if (i % freq == 0):
#            if not os.path.isdir('test'):
#                os.system('mkdir test')
#            np.savetxt("test/data_{:04d}.txt".format(i//freq), data)