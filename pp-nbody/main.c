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
    const char *file_name;   //name of initial conditions file
    const char *outputdir;  // dir of output files
    int N = 2;   //number of particles
    double epsilon = 0.0;  //softening parameter
    double t_step = 0.01;  //time step
    int n_steps = 5;   // number of steps
    int op_freq = 1;    // output frequency
    
    const char *integrator;  //ODE integration method
    
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
    else {
        printf("Usage: ./main fname outputdir N epsilon t_step n_steps op_freq [integrator]\n");
        return -1;
    }

    /* int N = 2;   //number of particles */
    /* float epsilon = 0.0;  //softening parameter */
    /* float t_step = 0.05;  //time step */
    /* int n_steps = 4836;   // number of steps */
    /* int op_freq = 4;    // output frequency */
    /* char *integrator = "LF2";  //ODE integration method */
    /* char *file_name = "init_2body0.5.txt";   //name of initial conditions file */

    /* int i; //loop variable */

    /* if (argc > 1) { */
    /*     if (argc != 8) { */
    /*         printf("Usage: ./main [N epsilon t_step n_steps op_freq integ fname]\n"); */
    /*         return -1; */
    /*     } */
    /*     //check user input */
    /*     for (i=0; i<strlen(argv[1]); i++) */
    /*         assert(isdigit(argv[1][i])); */
    /*     N = atoi(argv[1]); */
    /*     epsilon = atof(argv[2]); */
    /*     assert(epsilon >= 0.0); */
    /*     t_step = atof(argv[3]); */
    /*     assert(t_step >= 1e-8); */
    /*     for (i=0; i<strlen(argv[4]); i++) */
    /*         assert(isdigit(argv[4][i])); */
    /*     n_steps = atoi(argv[4]); */
    /*     for (i=0; i<strlen(argv[5]); i++) */
    /*         assert(isdigit(argv[5][i])); */
    /*     op_freq = atof(argv[5]); */
    /*     integrator = argv[6]; */
    /*     file_name = argv[7]; */
    /* } */

    float mass[N];
    float position[N][DIM];
    float velocity[N][DIM];
    float force[N][DIM];

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
