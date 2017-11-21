#ifndef integ_LF2_h
#define integ_LF2_h

#include "definitions.h"

void integ_LF2(int N, DATA *data, const NODE *root, double force[][DIM], double epsilon, double step)
{
    /*update position and velocity using 2nd order leapfrog*/

    int i, k; //loop vars

    //advance position by half a step
    for(i = 0; i < N; i++)
        for(k = 0; k < DIM; k++)
            data[i].pos[k] +=  0.5 * step * data[i].vel[k];


    //force after half a step
    calc_force(N, data, root, force, epsilon);

    //update velocity and position
    for(i = 0; i < N; i++)
        for(k = 0; k < DIM; k++)
        {
            data[i].vel[k] += step * force[i][k] / data[i].mass;
            data[i].pos[k] += 0.5 * step * data[i].vel[k];
        }
}

#endif
