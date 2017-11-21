#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Filename: energy.py
Description: A script to compute the total energy of a particle system.
Python is too slow for this job, so inside this script I use C code 
calenergy.c to do the heavy job
@author: chongchonghe
"""

from __future__ import division, print_function
import sys
import subprocess
import numpy as np
import matplotlib.pyplot as plt
import glob

N = 1000
dPath = "../simulations/cluster03"
files1 = sorted(glob.glob(dPath + "/*"))
indices = np.arange(0, len(files1), 6)

energy1 = []
for i in indices:
    filei = "{}/data_{:04d}.txt".format(dPath, i)
    out = subprocess.check_output("./calenergy {} {}".format(filei, N), shell=True)
    energy1.append(np.double(out.split()[8]))


"""
dPath = "cluster03_small_epsilon"
#files2 = sorted(glob.glob(dPath + "/*"))
energy2 = []
for i in indices:
    filei = "{}/data_{:04d}.txt".format(dPath, i)
    out = subprocess.check_output("./calenergy {} {}".format(filei, N), shell=True)
    energy2.append(np.double(out.split()[8]))

# plt.plot(indices, energy1, label="BH n-body")
# plt.plot(indices, energy2, label="PP n-body")
plt.plot(indices, energy1, label="epsilon = 0.005")
plt.plot(indices, energy2, label="epsilon = 0.00001")
plt.xlabel("steps")
plt.ylabel("total energy")
plt.legend()
plt.tight_layout()
plt.savefig("energy_compare_epsilon.pdf")
"""


#################################3

N = 1000
dPath = "cluster03_BH"
files3 = sorted(glob.glob(dPath + "/*"))
indices = np.arange(0, len(files3), 6)
energy3 = []
for i in indices:
    filei = "{}/data_{:04d}.txt".format(dPath, i)
    out = subprocess.check_output("./calenergy {} {}".format(filei, N), shell=True)
    energy3.append(np.double(out.split()[8]))

dPath = "cluster03_BH_theta3"
energy4 = []
for i in indices:
    filei = "{}/data_{:04d}.txt".format(dPath, i)
    out = subprocess.check_output("./calenergy {} {}".format(filei, N), shell=True)
    energy4.append(np.double(out.split()[8]))

plt.figure()
plt.plot(indices, energy1, label="PP-nbody")
plt.plot(indices, energy3, label="BH-nbody, theta = 0.6")
plt.plot(indices, energy4, label="BH-nbody, theta = 0.3")
plt.xlabel("steps")
plt.ylabel("total energy")
plt.legend()
plt.tight_layout()
plt.savefig("energy_compare_PP_BH.pdf")
