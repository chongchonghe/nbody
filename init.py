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

N = 1000
mass = imf(N)

def