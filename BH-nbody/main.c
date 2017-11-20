/*********************************************************
ASTR615 - HW#4
N-body code using Barnes & Hut method

Authors: Mohammed Khalil and ChongChong He

Date: 11/18/17

Filename: main.c
*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include "definitions.h"
#include "read_data.h"
#include "bhtree.h"
#include "force.h"
#include "integ_LF2.h"
#include "integ_RK4.h"
#include "save_data.h"

_Bool is_binary(double rTypical, double m1, double m2, double position1[DIM],
                double position2[DIM], double velocity1[DIM], double velocity2[DIM])
{
    // check distance < alpha * rTypical
    double alpha = 5.0;
    double beta = 2.0;
    double distSq = 0.0;
    
    for (int i = 0; i < DIM; i++)
        distSq += (position1[i] - position2[i]) * (position1[i] - position2[i]);
    if (distSq > alpha * alpha * rTypical * rTypical)
        return 0;
    
    // if close enough to each other, check K / |W| < 1 - beta^-1
    double reducedMass;
    double relaVelSq = 0.0;
    double kinetic;
    double potentialAbs;
    
    for (int i = 0; i < DIM; i++)
        relaVelSq += (velocity1[i] - velocity2[i]) * (velocity1[i] - velocity2[i]);
    reducedMass = m1 * m2 / (m1 + m2);
    kinetic = 0.5 * reducedMass * relaVelSq;
    potentialAbs = m1 * m2 / sqrt(distSq);
    if (kinetic / potentialAbs < 1.0 - 1.0 / beta)
        return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    // Usage: ./main fname outputdir N epsilon t_step n_steps op_freq [integrator]

    /*default parameters*/
    char *file_name = "cluster03.txt";   //name of initial conditions file
    char *outputdir = "./data/";  // dir of output files
    int N = 1000;   //number of particles
    double epsilon = 0.01;  //softening parameter
    double t_step = 0.01;  //time step
    int n_steps = 100;   // number of steps
    int op_freq = 1;    // output frequency
    char *integrator = "LF2";  //ODE integration method
    int i,j,k; //loop variables

	/*check user input*/
    if ((argc == 8) || (argc == 9)) {

        file_name = argv[1];

        outputdir = argv[2];

        for (i=0; i<strlen(argv[3]); i++)
            assert(isdigit(argv[3][i]));
        N = atoi(argv[3]);

        epsilon = atof(argv[4]);
        assert(epsilon >= 0.0);

        t_step = atof(argv[5]);
        assert(t_step >= 1e-8);

        for (i=0; i<strlen(argv[6]); i++)
            assert(isdigit(argv[6][i]));
        n_steps = atoi(argv[6]);

        for (i=0; i<strlen(argv[7]); i++)
            assert(isdigit(argv[7][i]));
        op_freq = atof(argv[7]);

        if (argc == 9)
            integrator = argv[9];
        else
            integrator = "LF2";
    }

    double force[N][DIM]; //force felt by each particle
	DATA data[N];   //mass, position, and vel data
	NODE *root;  //stores BH tree
    
    double rTypical = 

	read_data(N, data, file_name);  //read data from initial conditions file

    for(i = 0; i < n_steps; i++)
    {
        // check binarity
        for (i = 0; i < N; i++) {
            for (j = i + 1; j < N; j++) {
                if (is_binary(rTypical, data[i].mass, data[j].mass, data[i].pos, data[j].pos, data[i].vel, data[j].vel))
                    }
        }
        
        /* build tree */
        root = NULL;
        build_tree(N, data, &root);

        /*calculate force on each particle*/
        calc_force(N, data, root, force, epsilon);

        /*update position using RK4 or LF2*/
        if( strcmp(integrator,"RK4") == 0 )
            integ_RK4(N, data, root, force, epsilon, t_step);
        else if ( strcmp(integrator, "LF2") == 0 )
            integ_LF2(N, data, root, force, epsilon, t_step);
        else
            return -1;

        /*save current position*/
        if(i % op_freq == 0)
            save_data(N, data, (int)(i/op_freq), outputdir);

        /*free memory*/
        kill_node(root);
    }

	return 0;
}

