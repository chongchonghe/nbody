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

double cal_potential(int N, DATA *data)
{
    double pot = 0.0;
    double dis2;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            dis2 = 0.0;
            for (int k = 0; k < DIM; k++)
                dis2 += pow((data[i].pos[k] - data[j].pos[k]), 2.0);
            pot += - data[i].mass * data[j].mass / sqrt(dis2);
        }
    }
    return pot;
}

double cal_kinetic(int N, DATA *data)
{
    double kin = 0.0;
    for (int i = 0; i < N; i++)
        for (int k = 0; k < DIM; k ++)
            kin += data[i].mass * data[i].vel[k] * data[i].vel[k];
    return kin / 2.0;
}

_Bool is_close_encounter(double dVirial, double mass[], double position[][DIM],
                         double velocity[DIM])
{
    // check distance < alpha * dVirial
    const double alpha = 5.0;
    const double beta = 2.0;
    double dist2 = 0.0;
    
    for (int i = 0; i < DIM; i++)
        dist2 += (position1[i] - position2[i]) * (position1[i] - position2[i]);
    if (dist2 > alpha * alpha * dVirial * dVirial)
        return 0;

    // if close enough to each other, check T + V < 0
    double kinetic = 0.0;
    for (int i = 0; i < DIM; i++)
        kinetic += (velocity1[i] - velocity2[i]) * (velocity1[i] - velocity2[i]);
    kinetic /= 2.0;
    
}

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
