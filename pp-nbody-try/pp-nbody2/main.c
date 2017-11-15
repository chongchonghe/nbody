//
//  main.c
//  pp-nbody2
//
//  Created by ChongChong He on 11/14/17.
//  Copyright © 2017 ChongChong He. All rights reserved.
//

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
#include "save_data.h"

int DIM = 3;  //number of dimensions

int main(int argc, const char * argv[]) {
    const char *file_name;   //name of initial conditions file
    int N = 2;   //number of particles
    double epsilon = 0.0;  //softening parameter
    double t_step = 0.01;  //time step
    int n_steps = 5;   // number of steps
    int op_freq = 1;    // output frequency
    
    const char *integrator;  //ODE integration method
    
    int i; //loop variable
    
    if ((argc == 7) || (argc == 8)) {
        //check user input
        file_name = argv[1];
        for (i=0; i<strlen(argv[2]); i++)
            assert(isdigit(argv[2][i]));
        N = atoi(argv[2]);
        epsilon = atof(argv[3]);
        assert(epsilon >= 0.0);
        t_step = atof(argv[4]);
        assert(t_step >= 1e-8);
        for (i=0; i<strlen(argv[5]); i++)
            assert(isdigit(argv[5][i]));
        n_steps = atoi(argv[5]);
        for (i=0; i<strlen(argv[6]); i++)
            assert(isdigit(argv[6][i]));
        op_freq = atof(argv[6]);
        if (argc == 8)
            integrator = argv[7];
        else
            integrator = "LF2";
    }
    else {
        printf("Usage: ./main [fname N epsilon t_step n_steps op_freq [integrator]]\n");
        return -1;
    }
    
    double *mass;
    mass = (double *) malloc(N * sizeof(double));
    double **position, **velocity, **force;
    position = (double **) malloc(N * sizeof(double *));
    velocity = (double **) malloc(N * sizeof(double *));
    force = (double **) malloc(N * sizeof(double *));
    assert(position);
    assert(velocity);
    assert(force);
    for (i = 0; i < N; i++) {
        position[i] = (double *) malloc(DIM * sizeof(double));
        velocity[i] = (double *) malloc(DIM * sizeof(double));
        force[i] = (double *) malloc(DIM * sizeof(double));
        assert(position[i]);
        assert(velocity[i]);
        assert(force[i]);
    }

    read_data(mass, position, velocity, N, file_name);  //read initial pos and vel from file

    for(i = 0; i < n_steps; i++)
    {
        calc_force(mass, position, force, N, epsilon);
        
//        if( strcmp(integrator,"RK4") == 0 )
//            integ_RK4(mass, position, velocity, force, N, t_step, epsilon);
//        else if ( strcmp(integrator, "LF2") == 0 )
            integ_LF2(mass, position, velocity, force, N, t_step, epsilon);
//        else
//            return -1;
//
        if(i % op_freq == 0)
            save_data(mass, position, velocity, N, (int)(i/op_freq));
    }
    
    printf("hello\n");
    return 0;
}
