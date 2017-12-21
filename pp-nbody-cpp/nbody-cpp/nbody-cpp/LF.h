/*******************
Mohammed Khalil and ChongChong He

integ_LF2.h updates position and velocity using 2nd order leapfrog
********************/

#ifndef LF_h
#define LF_h

//extern int DIM;  //number of dimensions
#include "def.h"

//void LF(double mass[], double position[][DIM], double velocity[][DIM], double force[][DIM], int N, double step, double epsilon)
void LF(int N, double particle[][LEN], double force[][DIM], double step, double epsilon)
{
    /*update position and velocity using 2nd order leapfrog*/

    int i, k; //loop vars

    //advance position by half a step
    for(i = 0; i < N; i++)
        for(k = 0; k < DIM; k++)
//            position[i][k] += 0.5 * step * velocity[i][k];
            particle[i][1 + k] += 0.5 * step * particle[i][1 + DIM + k];

    //force after half a step
//    calc_force(mass, position, force, N, epsilon);
    calc_force(N, particle, force, epsilon);

    //update velocity and position
    for(i = 0; i < N; i++)
    {
        for(k = 0; k < DIM; k++)
        {
            particle[i][1 + DIM + k] += step * force[i][k] / particle[i][0];
            particle[i][1 + k] += 0.5 * step * particle[i][1 + DIM + k];
//            velocity[i][k] += step * force[i][k] / mass[i];
//            position[i][k] += 0.5 * step * velocity[i][k];
        }
    }
}

#endif
