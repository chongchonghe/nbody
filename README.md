# README

## Authors

Group 4 : ChongChong He and Mohammed Khalil 

## Description

ASTR615 HW #4: Simulation and Visualization of N-body Problem using particle-particle method and Barnes-Hut tree method

##############################################################

# Problem 1

The directory pp-nbody contains the particle-particle code
The directory BH-nbody contains the Barnes-Hut code

### To Compile
cd to either directories and 
    gcc -o main main.c

### To Run
./main fname outputdir N epsilon t_step n_steps op_freq [integrator]

For example, for 2 bodies with e = 0.5 (default parameters)
./main 2body0.5.txt ./data 2 0 0.05 4836 4 LF2

for 2 bodies with e = 0.9
./main 2body0.9.txt ./data 2 0 0.003 56547 50 RK4 

### To produce plots

Data files will be saved in the outputdir (./data)

Run p1_plots.py to produce orbit, phase, and energy plots

Change lines 8 and 9 in the file according to the used method and eccentricity

Plots will be saved to the directory ./plots_p1

### Files Description (in directory BH-nbody)

main.c   takes input from the user and runs the simulation
 
definitions.h contains structure and constant definitions

read_data.h   reads data from initial conditions file

bhtree.h      builds Barnes-Hut tree

force.h       performs tree walk and calculates force on each particle

integ_LF2.h   updates position and velocity using 2nd order leapfrog

integ Rk4.h   updates position and velocity using 4th order Runge-Kutta

save_data.h   saves data to output directory 

###############################################################

# Problem 2

Files are in the folders *simulation* and *visual*. 

cd into simulations/ and do

	../pp-nbody/a.out cluster03.txt cluster03 1000 0.01 0.001 200 1

and all the outputs will be in the folder cluster03/.

visual_parallel.py runs in parallel and make plots of all the outputs in a given folder. You need the following packages: mayavi and mpi4py. They are not easy to install. The videos are shown in the folder simulations/cluster03.mp4


