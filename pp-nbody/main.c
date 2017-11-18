/*********************************************************
ASTR615 - HW#4 Problem #2
Particle-Particle N-body code

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

#include "read_data.h"
#include "calc_force.h"
#include "integ_LF2.h"
#include "integ_RK4.h"
#include "save_data.h"

int DIM = 3;  //number of dimensions

int main(int argc, char *argv[])
{
    char *file_name = "init_2body0.5.txt";;   //name of initial conditions file
    char *outputdir = "./data/";  // dir of output files
    int N = 2;   //number of particles
    double epsilon = 0.0;  //softening parameter
    double t_step = 0.05;  //time step
    int n_steps = 4836;   // number of steps
    int op_freq = 4;    // output frequency
    char *integrator = "RK4";  //ODE integration method
    int i; //loop variable

    if ((argc == 8) || (argc == 9)) {
        //check user input

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
//    else {
//        printf("Usage: ./main fname outputdir N epsilon t_step n_steps op_freq [integrator]\n");
//        return -1;
//    }

    double mass[N];
    double position[N][DIM];
    double velocity[N][DIM];
    double force[N][DIM];

    read_data(mass, position, velocity, N, file_name);  //read initial pos and vel from file

    for(i = 0; i < n_steps; i++)
    {
        calc_force(mass, position, force, N, epsilon);

        if( strcmp(integrator,"RK4") == 0 )
            integ_RK4(mass, position, velocity, force, N, t_step, epsilon);
        else if ( strcmp(integrator, "LF2") == 0 )
            integ_LF2(mass, position, velocity, force, N, t_step, epsilon);
        else
            return -1;

        if(i % op_freq == 0)
            save_data(mass, position, velocity, N, (int)(i/op_freq), outputdir);
    }

    return 0;
}
