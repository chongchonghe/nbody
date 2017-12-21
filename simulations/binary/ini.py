#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""

@author: chongchonghe
"""

from __future__ import division, print_function
import numpy as np
import matplotlib.pyplot as plt

def f(theta, beta):
    theta = theta / 180 * np.pi
    v1c = np.sqrt(50 * ( 1 - beta**-1))
    u1 = np.array([v1c * np.sin(theta), v1c * np.cos(theta), 0])
    v1 = u1 + np.array([0, 10, 0])

    u2 = np.array([- v1c * np.sin(theta), - v1c * np.cos(theta), 0])
    v2 = u2 + np.array([0, 10, 0])
    data = np.zeros([3, 7])
    data[0, :] = [1000, 0, 0, 0, 0, 0, 0]
    data[1, :4] = [10, 9.9, 0, 0]
    data[2, :4] = [10, 10.1, 0, 0]
    data[1, 4:] = v1
    data[2, 4:] = v2
    np.savetxt("test03.txt", data)
    print("v1 =", v1)
    print("v2 =", v2)

f(60, 5)
