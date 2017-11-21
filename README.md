# README

## Authors

Mohammed Khalil, ChongChong He

## Description

ASTR615 HW #4: Simulation and Visualization of N-body Problems

## How to run

### Problem 2

Files are in the folders *simulation* and *visual*. 

cd into simulations/ and do

	../pp-nbody/a.out cluster03.txt cluster03 1000 0.01 0.001 200 1

and all the outputs will be in the folder cluster03/.

visual_parallel.py runs in parallel and make plots of all the outputs in a given folder. You need the following packages: mayavi and mpi4py. They are not easy to install. The videos are shown in the folder simulations/cluster03.mp4

## TODO
1. Plot of space diagram (x vs. y) needs to have scale. Ues ax.axis('equal')
2. F ~ r / (r^2 + epsilon^2)^(3/2), instead of r/|r| * 1/(r^2 + epsilon^2). I've changed this in pp-nbody and BH-nbody under "binary-BH" branch.
3. 
