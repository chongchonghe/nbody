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
#include "is_binary.h"

#include "calc_PP_force.h"
#include "../energy.h"

// comment the following line to switch to PP-nbody
//#define BH
// Uncomment the following line to open binary finder
#define BINARY

//#ifndef BH
//#define CALFORCE calc_PP_force(N, data, force, epsilon)
//#else
//#define CALFORCE calc_force(N, data, root, force, epsilon)
//#endif

//#ifdef BINARY
//#define FINDBINARY

int main(int argc, char *argv[])
{
    // Usage: ./main fname outputdir N epsilon t_step n_steps op_freq [integrator]

    /*default parameters*/
    char *file_name = "binary/test.txt";   //name of initial conditions file
    char *outputdir = "binary/test3/";  // dir of output files
    int N = 3;   //number of particles
    double epsilon = 2e-5;  //softening parameter
    double t_step = 0.01;  //time step
    int n_steps = 100;   // number of steps
    int op_freq = 1;    // output frequency
    int integrator = 1;  //ODE integration method, 1: LF2, 2: RK4
    int i; //loop variables
    int binarity[N];
    
    for (i = 0; i < N; i++)
        binarity[i] = 1;

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
            if (strcmp(argv[8], "RK4") == 0)
                integrator = 2;
    }
    else if (argc != 1)
    {
        printf("usage: ./main fname outputdir N epsilon t_step n_steps op_freq [integrator]");
        return -1;
    }

    double force[N][DIM]; //force felt by each particle
	DATA data[N];   //mass, position, and vel data
	NODE *root;  //stores BH tree
    
    double dClose = 0.2 * 2; // TODO
    double mTwo;

	read_data(N, data, file_name);  //read data from initial conditions file

    printf("Inital totoal energy: %lf\n", cal_potential(N, data) + cal_kinetic(N, data));
    
    for(i = 0; i < n_steps; i++)
    {
        
        // check binarity
        for (int ii = 0; ii < N; ii++)
        {
            for (int jj = ii + 1; jj < N; jj++)
            {
                if (is_binary(dClose, data[ii].mass, data[jj].mass, data[ii].pos, data[jj].pos, data[ii].vel, data[jj].vel) && (binarity[ii] != 0) && (binarity[jj] != 0))
                {
                    mTwo = data[ii].mass + data[jj].mass;
                    for (int k = 0; k < DIM; k++)
                    {
                        data[ii].pos[k] = (data[ii].mass * data[ii].pos[k] + data[jj].mass * data[jj].pos[k]) / mTwo;
                        data[ii].vel[k] = (data[ii].mass * data[ii].vel[k] + data[jj].mass * data[jj].vel [k]) / mTwo;
                    }
                    data[ii].mass = mTwo;
                    data[jj].mass = 1e-8;
                    binarity[ii]++;
                    binarity[jj] = 0;
                }
                //                        printf("binary found? Yes. %d and %d\n", ii, jj);
                //                    else
                //                        printf("binary found? No\n");
            }
        }
        
        /*save current position*/
        if(i % op_freq == 0)
        {
            save_data(N, data, (int)(i/op_freq), outputdir, binarity);
        }
        
        /* build tree */
        root = NULL;
        build_tree(N, data, &root);

        /*calculate force on each particle*/
        calc_force(N, data, root, force, epsilon);
//        CALFORCE;

        /*update position using RK4 or LF2*/
        if (integrator == 1)
            integ_LF2(N, data, root, force, epsilon, t_step);
        else if (integrator == 2)
            integ_RK4(N, data, root, force, epsilon, t_step);
        else
            return -1;

        /*free memory*/
        kill_node(root);
    }
    
    printf("Final totoal energy: %lf\n", cal_potential(N, data) + cal_kinetic(N, data));

	return 0;
}
