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
from scipy import optimize
#import plotstyle
import matplotlib.pyplot as plt

# Lengths are in units of kpc
# Milky Way Galaxy

def rx(x, rH):
    """ return the solution for x = 1 - (1 + r/rh) * exp(-r/rh) """
    g = lambda t: 1 - (1 + t) * np.exp(-t) - x
    return optimize.brentq(g, 0, 1e5) * rH

def ExpRandom(N, h):
    """ generate random numbers according to rho(r) = r(0) * exp(-r/h) """
    x = np.random.random(N)
    sample = [rx(xx, h) for xx in x]
    return sample

###############
    
#def ExpRandom(N, h):
#    x = np.random.random(N)
#    sample = []
#    for xx in x:
#        if 
#        sample.append(- np.log(x) * h)
#    return sample