#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Filename: energy.py
Description: A script to compute the total energy of a particle system.
@author: chongchonghe
"""

from __future__ import division, print_function
import sys
import subprocess
import numpy as np
import matplotlib.pyplot as plt


def main(dPath):

    files = glob.glob(dPath + "/*")
    count = 0
    for filei in files:
        subprocess.check_output("./calenergy {}".format(filei), shell=True)
    

if __name__ == "__main__":
    dPath = sys.argv[1]
    main(dPath)
