#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""

@author: chongchonghe
"""

from __future__ import division, print_function
import numpy as np
# import matplotlib
# matplotlib.use('Agg')
import glob
import time
import sys
import os
import matplotlib.pyplot as plt
# import matplotlib.animation as animation
from matplotlib import animation

plt.style.use('dark_background')
fig = plt.figure()

ax = plt.axes(xlim=(-3.4, 3.4), ylim=(-1.8, 3.2))
ax.axis('off')

N = 3
lines = [plt.plot([], [], '.')[0] for _ in range(N)]

def init():    
    for line in lines:
        line.set_data([], [])
    return lines

startid = 0
files = sorted(glob.glob(sys.argv[1] + "/*"))[startid:]

def animate(i):
    fn = files[i]
    data = np.loadtxt(fn)
    N = len(data)
    x = data[:, 1]
    y = data[:, 2]
    try:
        binarity = data[:, 7]
    except IndexError:
        binarity = None;

    for j, line in enumerate(lines):
        isPlot = False
        if binarity is None:
            cid = 0
            isPlot = True
        elif binarity[j] >= 1:
            cid = int(binarity[j])
            isPlot = True
        if isPlot:
            line.set_data(x[j], y[j]) # , "C{:d}.".format(cid))

    return lines

anim = animation.FuncAnimation(fig, animate, init_func=init,
                               frames=len(files), interval=50, blit=True)

plt.show()


sys.exit()


if len(sys.argv) <= 2:
    raise SystemExit("usage: ./movie2d.py datadir boxsize [num of fig]")
if len(sys.argv) == 4:
    numOfFig = int(sys.argv[3])
    if (numOfFig != 1) and (numOfFig != 2):
        raise SystemExit("num of fig must be 1 or 2")
else:
    numOfFig = 2

plt.style.use('dark_background')

if numOfFig == 1:
    fig, ax = plt.subplots(1, figsize=[6, 6])
elif numOfFig == 2:
    fig, ax = plt.subplots(1, 2, figsize=[12, 6])

boxlen = np.double(sys.argv[2])
lims = [-boxlen, boxlen, -boxlen, boxlen]
if len(sys.argv) == 4:
    startid = int(sys.argv[3])
else:
    startid = 0

outdir = os.path.abspath(sys.argv[1]) + "_fig"
if not os.path.isdir(outdir):
    os.system("mkdir {}".format(outdir))

idx = 0
files = sorted(glob.glob(sys.argv[1] + "/*"))[startid:]

if numOfFig == 2:
    ax[1].plot([0, len(files)], [1, 1])
    xlim = ax[1].get_xlim()
    ax[1].cla()

for fn in files:
    data = np.loadtxt(fn)
    N = len(data)
    x = data[:, 1]
    y = data[:, 2]
    try:
        binarity = data[:, 7]
    except IndexError:
        binarity = None;
    
    if numOfFig == 1:
        ax1 = ax
    else:
        ax1 = ax[0]
    ax1.cla()
    ax1.axis('equal')
    ax1.axis(lims)
    #ax1.plot(x[:-1], y[:-1], '.')
    #ax1.plot(x[-1], y[-1], '.')
    for i in range(N):
        isPlot = False
        if binarity is None:
            cid = 0
            isPlot = True
        elif binarity[i] >= 1:
            cid = int(binarity[i])
            isPlot = True
        if isPlot:
            ax1.plot(x[i], y[i], "C{:d}.".format(cid))
    # ax1.plot(x[1], y[1], '.')
    # ax1.plot(x[2], y[2], '.')
    ax1.set_xlabel("x")
    ax1.set_ylabel("y")
    ax1.set_title("phase plot")
    #plt.draw()

    if numOfFig == 2:
        # plot distance of last two particles in log scale
        ax[1].set_yscale('log')
        ax[1].set_ylim([1e-2, 1e2])
        ax[1].set_xlim(xlim)
        ax[1].plot(idx, np.linalg.norm(data[-2, 1:4] - data[-1, 1:4]),'r.')
        ax[1].set_xlabel('steps')
        ax[1].set_ylabel('separation')
        ax[1].set_title("Binary separation")

    plt.savefig("{}/data_{:04d}.jpg".format(outdir, idx))
    idx += 1
