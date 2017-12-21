#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
energy.py

Description: A script to compute the total energy of a particle system.
Python is too slow for this job, so inside this script I use C code 
calenergy.c to do the heavy job

Notice: This script may not work without all necessary data files.

Author: ChongChong He
"""

from __future__ import division, print_function
import sys
import subprocess
import numpy as np
import matplotlib.pyplot as plt
import glob


N = 1000 # the number of particles in all data files

def plot_energy(ax, dPath, label):
    # dPath = "../simulations/cluster_v2"
    files1 = sorted(glob.glob(dPath + "/*"))
    indices = np.arange(0, len(files1), 6)

    # collect the energy from cluster
    energy1 = []
    for i in indices:
        filei = "{}/data_{:04d}.txt".format(dPath, i)
        out = subprocess.check_output("./calenergy {} {}".format(filei, N), shell=True)
        energy1.append(np.double(out.split()[8]))
    ax.plot(indices, energy1, label=label)

fig, ax = plt.subplots()

clusters = ["cluster", "cluster_v3", "cluster_v4", "cluster_v5",
            "cluster_v6", "cluster_v7", "cluster_v8"]
labels = [r"$\epsilon=0.005, h=0.0005$", r"$\epsilon=0.003, h=0.0005$",
          r"$\epsilon=0.006, h=0.0005$", r"$\epsilon=0.005, h=0.0002$",
          r"$\epsilon=0.003, h=0.0002$", r"$\epsilon=0.006, h=0.0002$",
          r"$\epsilon=0.008, h=0.0002$"]
for dPath, label in zip(clusters, labels):
    plot_energy(ax, dPath, label)

ax.set_xlabel("steps")
ax.set_ylabel("total energy")
plt.legend()
plt.tight_layout()
plt.savefig("energy_compare.pdf")
# plt.savefig("energy_compare_epsilon.pdf")


# # collect the energy from cluster_small_epsilon
# dPath = "cluster_small_epsilon"
# energy2 = []
# for i in indices:
#     filei = "{}/data_{:04d}.txt".format(dPath, i)
#     out = subprocess.check_output("./calenergy {} {}".format(filei, N), shell=True)
#     energy2.append(np.double(out.split()[8]))
# plt.plot(indices, energy2, label="epsilon = 0.00001")


# # collect the energy from cluster_BH
# dPath = "cluster_BH"
# files3 = sorted(glob.glob(dPath + "/*"))
# indices = np.arange(0, len(files3), 6)
# energy3 = []
# for i in indices:
#     filei = "{}/data_{:04d}.txt".format(dPath, i)
#     out = subprocess.check_output("./calenergy {} {}".format(filei, N), shell=True)
#     energy3.append(np.double(out.split()[8]))


# # collect the energy from cluster_BH_theta3
# dPath = "cluster_BH_theta3"
# energy4 = []
# for i in indices:
#     filei = "{}/data_{:04d}.txt".format(dPath, i)
#     out = subprocess.check_output("./calenergy {} {}".format(filei, N), shell=True)
#     energy4.append(np.double(out.split()[8]))

# # make a plot
# plt.figure()
# plt.plot(indices, energy1, label="PP-nbody")
# plt.plot(indices, energy3, label="BH-nbody, theta = 0.6")
# plt.plot(indices, energy4, label="BH-nbody, theta = 0.3")
# plt.xlabel("steps")
# plt.ylabel("total energy")
# plt.legend()
# plt.tight_layout()
# plt.savefig("energy_compare_PP_BH.pdf")
