#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Nov 15 11:40:36 2017

@author: chongchonghe
"""

from __future__ import print_function, division
import numpy as np
import matplotlib.pyplot as plt
from astropy.units import M_sun
from astropy.constants import M_sun, G

fpath = '/Users/chongchonghe/Sam/Job2.2/output_00044/sink_00044.csv'
#fpath = "/Users/chongchonghe/Sam/Job1.2/output_00008/sink_00008.csv"
data = np.loadtxt(fpath, delimiter=",")
data = data[:, 1:8]

# get rid of small stars
data = data[(data[:, 0] > 1e-2), :]

m = data[:, 0]
x = data[:, 1]
vx = data[:, 4]

np.savetxt("cluster01.txt", data)

sigma2 = np.sum(data[:, 4]**2 + data[:, 5]**2 + data[:, 6]**2) / 3 / len(data[:, 0])
sigma = np.sqrt(sigma2)
boxlen = (x.max() - x.min()) / 2
print("N =", len(data))
print("boxlen =", boxlen)
T = boxlen / sigma
print("The dispersion crossing time is", T)

# Units
u_l = 0.308000000000000E+19
u_rho = 0.232474340000000E-23
u_t = 0.253950790000000E+16
u_m = u_l**3 * u_rho
msun = M_sun.cgs.value
u_msum = u_m / msun
G = G.cgs.value

# check virial status
K = 0.5 * np.sum(m) * sigma2
W = np.sum(m)**2 / boxlen
virial = 2 * K / W      # ~800
ratio = u_l**3 / unit_t**2 / u_m / G
#boxlen = 20
virial2 = sigma2 * boxlen / np.sum(m)
print("Virial ratio =", virial, virial * ratio, virial2)