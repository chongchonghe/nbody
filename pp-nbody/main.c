// Change log:
// Add check_binary, is_close_encounter
// Add a print to file at the inital step


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

//_Bool is_close_encounter(double rTypical, double m1, double m2, double position1[DIM], double position2[DIM],
//                         double velocity1[DIM], double velocity2[DIM])
//{
//    // check distance < alpha * rTypical
//    double alpha = 2.0;
//    double dist = 0.0;
//    for (int i = 0; i < DIM; i++)
//        dist += (position1[i] - position2[i]) * (position1[i] - position2[i]);
//    if (dist * dist > alpha * rTypical * rTypical)
//        return 0;
//
//    // if close enough to each other, check T + V < 0
//    double kinetic = 0.0;
//    for (int i = 0; i < DIM; i++)
//        kinetic += (velocity1[i] - velocity2[i]) * (velocity1[i] - velocity2[i]);
//
//
//}

//void check_binary(double mass[], double position[][DIM], double force[][DIM])
//{
//    int i, j, k;
//    for (i = 0; i < N; i++) {
//        for (j = i + 1; j < N; j++) {
//            if is_close_encounter {
//
//            }
//        }
//    }
//}

int main(int argc, char *argv[])
{
    char *file_name = "binary/test01.txt";;   //name of initial conditions file
    char *outputdir = "binary/test01";  // dir of output files
    int N = 2;   //number of particles
    double epsilon = 0.001;  //softening parameter
    double t_step = 0.1;  //time step
    int n_steps = 20;   // number of steps
    int op_freq = 1;    // output frequency
    char *integrator = "LF2";  //ODE integration method
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
        if(i % op_freq == 0)
            save_data(mass, position, velocity, N, (int)(i/op_freq), outputdir);

        calc_force(mass, position, force, N, epsilon);

        if( strcmp(integrator,"RK4") == 0 )
            integ_RK4(mass, position, velocity, force, N, t_step, epsilon);
        else if ( strcmp(integrator, "LF2") == 0 )
            integ_LF2(mass, position, velocity, force, N, t_step, epsilon);
        else
            return -1;
    }

    return 0;
}
