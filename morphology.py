#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
morphology.py
A script that defines functions to produce the morphology of galaxies or 
stellar clusters.
Created on Sun Nov 12 00:31:51 2017
@author: chongchonghe
"""

from __future__ import print_function, division
import numpy as np
import plotstyle
import matplotlib.pyplot as plt

# Lengths are in units of kpc
# Milky Way Galaxy

def rx(x, rH):
    """ return the solution for x = 1 - (1 + r/rh) * exp(-r/rh) """
    g = lambda t: 1 - (1 + t) * np.exp(-t) - x
    return sp.optimize.brentq(g, 0, 1e5) * rH

def ExpRandom(N, h):
    """ generate random numbers according to rho(r) = r(0) * exp(-r/h) """
    x = np.random.random(N)
    sample = [rx(xx, h) for xx in x]
    return sample

N = 1000
scaleH = 10
scaleR = 100
r = ExpRandom(N, scaleR)
H = ExpRandom(N, scaleH)
theta = np.random.random(N) * 2 * np.pi
x = r * np.cos(theta)
y = r * np.sin(theta)
z = H
