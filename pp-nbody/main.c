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

// uncomment the following line to use RK4
// #define RK4

// #ifndef RK4

int DIM = 3;  //number of dimensions

int main(int argc, char *argv[])
{
    // Usage: ./main [N epsilon t_step n_steps op_freq integ fname]
    // struct Parameters pars = get_pars(); //read parameters from the user
    int N = 3;   //number of particles
    float epsilon = 0.0;  //softening parameter
    float t_step = 0.01;  //time step
    int n_steps = 3000;   // number of steps
    int op_freq = 100;    // output frequency
    char *integrator = "LF2";  //ODE integration method
    char *file_name = "data000.txt";   //name of initial conditions file

    int i;

    if (argc > 1) {
        if (argc != 8) {
            printf("Usage: ./main [N epsilon t_step n_steps op_freq integ fname]\n");
            return -1;
        }
        for (i=0; i<strlen(argv[1]); i++)
            assert(isdigit(argv[1][i]));
        N = atoi(argv[1]);
        epsilon = atof(argv[2]);
        assert(epsilon >= 0.0);
        t_step = atof(argv[3]);
        assert(t_step >= 1e-8);
        for (i=0; i<strlen(argv[4]); i++)
            assert(isdigit(argv[4][i]));
        n_steps = atoi(argv[4]);
        for (i=0; i<strlen(argv[5]); i++)
            assert(isdigit(argv[5][i]));
        op_freq = atof(argv[5]);
        integrator = argv[6];
        file_name = argv[7];
    }

    float mass[N];
    float position[N][DIM];
    float velocity[N][DIM];
    float force[N][DIM];

    read_data(mass, position, velocity, N, file_name);  //read initial pos and vel from file

    for(int i = 0; i < n_steps; i++)
    {
        calc_force(mass, position, force, N, epsilon);

        if( strcmp(integrator,"RK4") == 0 )
            integ_RK4(mass, position, velocity, force, N, t_step, epsilon);
       else if ( strcmp(integrator, "LF2") == 0 )
            integ_LF2(mass, position, velocity, force, N, t_step);
       else
            return -1;

        if(i % op_freq == 0)
            save_data(mass, position, velocity, N, (int)(i/op_freq));
    }

    return 0;
}
