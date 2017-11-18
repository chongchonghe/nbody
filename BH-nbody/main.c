/*********************************************************
ASTR615 - HW#4 Problem #2
N-body code using Barnes-Hut algorithm.

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
//#include "integ_RK4.h"
#include "save_data.h"


int main(int argc, char *argv[])
{
    // Usage: ./main [N epsilon t_step n_steps op_freq integ fname]

    char *file_name = "init_2body0.5.txt";;   //name of initial conditions file
    char *outputdir = "./data/";  // dir of output files
    int N = 2;   //number of particles
    double epsilon = 0.0;  //softening parameter
    double t_step = 0.05;  //time step
    int n_steps = 4836;   // number of steps
    int op_freq = 4;    // output frequency
    char *integrator = "LF2";  //ODE integration method
    int i,j,k; //loop variables

    double force[N][DIM];

	DATA data[N];
	NODE *root = NULL;

	read_data(N, data, file_name);

    for(i = 0; i < n_steps; i++)
    {
        /* build tree */
        build_tree(N, data, &root);

//        printf("Tree dimensions: (%g,%g,%g) to (%g,%g,%g)\n",
//		   root->cmin[0], root->cmin[1], root->cmin[2],
//		   root->cmax[0], root->cmax[1], root->cmax[2]);
//        printf("System total mass = %g\n", root->mass);
//        printf("System center of mass = (%g,%g,%g)\n",
//		   root->cog[0], root->cog[1], root->cog[2]);

        /*calculate force on each particle*/
        calc_force(N, data, root, force, epsilon);

//        printf("\nForce: ");
//        for(j = 0; j < N; j++)
//            for(k = 0; k < DIM; k++)
//                printf("%f  ", force[j][k]);

        /*update position*/
        integ_LF2(N, data, root, force, epsilon, t_step);
//        if( strcmp(integrator,"RK4") == 0 )
//            integ_RK4(mass, position, velocity, force, N, t_step, epsilon);
//        else if ( strcmp(integrator, "LF2") == 0 )
//            integ_LF2(mass, position, velocity, force, N, t_step, epsilon);
//        else
//            return -1;
//
//        /*save current position*/
        //if(i % op_freq == 0)
            save_data(N, data, (int)(i/op_freq), outputdir);
        // remove the tree from memory
        kill_node(root);
    }

	return 0;
}

/* bhtree.c */
