#!/bin/bash


#cluster02:
#../pp-nbody/a.out cluster02.txt cluster02v3 1000 0.01 0.0005 1000 1 


# cluster02_0.01:
#../pp-nbody/a.out cluster02.txt cluster02v3 1000 0.01 ? 10 1


# cluster02_0.001:
#../pp-nbody/a.out cluster02.txt cluster02v3 1000 0.001 ? 10 1


# cluster03

##### compare different epsilons #####
# ../pp-nbody/main cluster.txt cluster 1000 0.005 0.0005 400 1
# ../pp-nbody/main cluster.txt cluster_v2 1000 0.002 0.0005 400 1
# ../pp-nbody/main cluster.txt cluster_v3 1000 0.003 0.0005 400 1
# ../pp-nbody/main cluster.txt cluster_v4 1000 0.006 0.0005 400 1
# ../pp-nbody/main cluster.txt cluster_v4 1000 0.006 0.0005 400 1

##### compare different step sizes #####
# ../pp-nbody/main cluster.txt cluster_v5 1000 0.005 0.0002 1000 5
# ../pp-nbody/main cluster.txt cluster_v6 1000 0.003 0.0002 1000 5
# ../pp-nbody/main cluster.txt cluster_v7 1000 0.006 0.0002 1000 5
../pp-nbody/main cluster.txt cluster_v8 1000 0.008 0.0002 1000 5



# proper stepsize but small epsilon
#../pp-nbody/main cluster03.txt cluster03_small_epsilon 1000 0.00001 0.0005 400 1

# proper stepsize and epsilon, BH-nbody, theta_critical = 0.6
#../BH-nbody/main cluster03.txt cluster03_BH 1000 0.005 0.0005 400 1

# proper stepsize and epsilon, BH-nbody, theta_critical = 0.2
#../BH-nbody/main_theta2 cluster03.txt cluster03_BH_theta2 1000 0.005 0.0005 400 1
