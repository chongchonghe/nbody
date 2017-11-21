#!/bin/bash


#cluster02:
#../pp-nbody/a.out cluster02.txt cluster02v3 1000 0.01 0.0005 1000 1 


#cluster02_0.01:
#../pp-nbody/a.out cluster02.txt cluster02v3 1000 0.01 ? 10 1


#cluster02_0.001:
#../pp-nbody/a.out cluster02.txt cluster02v3 1000 0.001 ? 10 1


# cluster03

# proper stepsize and epsilon
#../pp-nbody/main cluster03.txt cluster03 1000 0.005 0.0005 400 1

# proper stepsize but small epsilon
#../pp-nbody/main cluster03.txt cluster03_small_epsilon 1000 0.00001 0.0005 400 1

# proper stepsize and epsilon, BH-nbody
../BH-nbody/main cluster03.txt cluster03_BH 1000 0.005 0.0005 4 1
