#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "read_data.h"
#include "calc_force.h"
#include "integ_LF2.h"
#include "integ_RK4.h"
#include "save_data.h"

int DIM = 3;  //number of dimensions

void read_data(float mass[], float position[][DIM], float velocity[][DIM], int N, char file_name[]);
void calc_force(float mass[], float position[][DIM], float force[][DIM], int N, float epsilon);
void integ_LF2(float mass[], float position[][DIM], float velocity[][DIM], float force[][DIM], int N, float step);
void integ_RK4(float mass[], float position[][DIM], float velocity[][DIM], float force[][DIM], int N, float step, float epsilon);
void save_data(float mass[], float position[][DIM], float velocity[][DIM], int N, int file_num);

struct Parameters{
    /*structure to save simulation parameters*/
    int N;   //number of particles
    float epsilon;  //softening parameter
    float t_step;  //time step
    int n_steps;   // number of steps
    int op_freq;    // output frequency
    char file_name[15];   //name of initial conditions file
    char integrator[3];  //ODE integration method
};

struct Parameters get_pars(); //function that gets parameters from user

int main()
{
    struct Parameters pars = get_pars(); //read parameters from the user
    int N = pars.N;
    float mass[N];
    float position[N][DIM];
    float velocity[N][DIM];
    float force[N][DIM];

    read_data(mass, position, velocity, N, pars.file_name);  //read initial pos and vel from file

    for(int i = 0; i < pars.n_steps; i++)
    {
        calc_force(mass, position, force, N, pars.epsilon);

        if( strcmp(pars.integrator,"RK4") == 0 )
            integ_RK4(mass, position, velocity, force, N, pars.t_step, pars.epsilon);
        else
            integ_LF2(mass, position, velocity, force, N, pars.t_step);

        if(i % pars.op_freq == 0)
            save_data(mass, position, velocity, N, (int)(i/pars.op_freq));
    }

    return 0;
}

struct Parameters get_pars(){
    /*reads simulation parameters from user*/
    struct Parameters pars = {3, 0, 0.01, 10000, 100, "data000.txt", "LF2"}; //defaults

    char *input[2];
    int int_inpt = -1;

    while (int_inpt != 0 || int_inpt != 1)
    {
        printf("Do you want to enter simulation parameters? \n[0] Yes\n[1] No\n");
        fgets(input, sizeof(input), stdin);
        sscanf (input, "%d", &int_inpt);
        if(int_inpt == 0 || int_inpt == 1)
            break;
    }

    if (int_inpt == 0){
        printf("Number of particles: ");
        scanf("%d", &pars.N);

        printf("Softening Parameter: ");
        scanf("%f", &pars.epsilon);

        printf("Time step: ");
        scanf("%f", &pars.t_step);

        printf("Number of steps: ");
        scanf("%d", &pars.n_steps);

        printf("Number of steps between outputs: ");
        scanf("%d", &pars.op_freq);

        printf("Name of initial conditions file: ");
        scanf("%s", pars.file_name);

        printf("Integration method ['LF2' or 'RK4']: ");
        scanf("%s", pars.integrator);
    }

    return pars;
}

