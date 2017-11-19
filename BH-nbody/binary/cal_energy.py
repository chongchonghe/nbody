import sys
import numpy as np

dPath = sys.argv[1]
data = np.loadtxt(dPath)
r = np.linalg.norm(data[1:4, 0] - data[1:4, 1])
W = - (data[0, 0] * data[1, 0]) / r
K = 0.5 * data[0, 0] * np.sum((data[0, 4:7] - data[1, 4:7])**2)
