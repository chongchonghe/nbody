#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
kroupa.py
A script that defines functions to generate particle masses according to 
Kroupa IMF.
Created on Sat Nov 11 12:10:22 2017
@author: chongchonghe
"""

from __future__ import print_function, division
import numpy as np
#import plotstyle
import matplotlib.pyplot as plt

def space(N):
    # Generate uniformly distributed particles inside a sphere, then stretched into an ellipsoid.
    # x^2/a^2 + y^2/b^2 + z^2/c^2 = 1
    semiaxis = [1, 1, 1]
    x, y, z = [np.random.random(3 * N) * 2 * semiax - semiax for semiax in semiaxis]
    isInside = x**2/semiaxis[0]**2 + y**2/semiaxis[1]**2 + z**2/semiaxis[2]**2 <= 1
    assert(np.sum(isInside) > N)
    x = x[isInside][:N]
    y = y[isInside][:N]
    z = z[isInside][:N]
    data = np.zeros([3, N])
    for i, xx in zip(range(3), [x, y, z]):
        data[i, :] = xx
    data = np.transpose(data)
    return data


def kroupa(x):
    """ Kroupa IMF after transformation. Imput unifrom random numbers 
    between 0 and 1; output the masses of sample stars """
    if (x>=0.0)&(x<0.760707):
        return -0.566179/((1.7987 - x)**(1/3.0) * (-5.81939 + 9.70599 * x - 
                         5.39611 * x**2 + x**3))
    elif (0.760707<=x)&(x<=1.0):
        return 0.166558/(1.00024 - x)**(10/13.0)
    else:
        return None


def imf(N):
    """ Generate the IMF """
    x = np.random.random(N)
    Vkroupa = np.vectorize(kroupa)
    m = Vkroupa(x)
    return m


def imf_theory(m, N):
    """ the histogram of Kroupa IMF in the form of a function """
    A = N / 3.9536
    m1 = 0.08
    m2 = 0.5
    m3 = 100
    if (m1-1e-8<=m)&(m<m2):
        return A * 2.30259 * m**(-0.3)
    elif (m2<=m)&(m<=m3+1e-8):
        return A * 1.15129 * m**(-1.3)
    else:
        return None


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


def main(N):
    # plot a histogram of the samples
    mass = imf(N)
    data = np.zeros([N, 7])
    data[:, 0] = mass
    data[:, 1:4] = space(N)
    data[:, 4:7] = set_vel(data, 0.8, 1)
    #np.savetxt("cluster03.txt", data)
    np.savetxt("cluster03_2000.txt", data)
    
    bins = np.logspace(np.log10(0.08), 2, 36)
    dlogm = np.log10(bins[1]) - np.log10(bins[0])
    plt.hist(mass, bins=bins, label="sample", weights=np.ones(len(mass))/dlogm)
    ax = plt.gca()
    ax.set_xscale('log')
    ax.set_yscale('log')
    
    # plot a line to compare with
    bins = np.logspace(np.log10(0.08), 2, 200)
    Vimf_theory = np.vectorize(imf_theory)
    mass_theory = Vimf_theory(bins, N)
    ax.plot(bins, mass_theory, label="Kroupa")
    plt.legend()
    plt.xlabel(r"$m/M_\odot$")
    plt.ylabel(r"$\frac{dN}{d\log(m/M_\odot)}$")
    plt.tight_layout()
    plt.savefig('imf.eps')

if __name__ == "__main__":
    N = 2000
    main(N)
