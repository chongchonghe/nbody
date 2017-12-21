/*******************
Mohammed Khalil and ChongChong He

calc_force.h calculates force on each particle using particle-particle method
********************/
#ifndef calc_force_h
#define calc_force_h

//#include "definitions.h"
#include "def.h"

//void calc_force(double mass[], double position[][DIM], double force[][DIM], int N, double epsilon)
void calc_force(int N, double particle[][LEN], double force[][DIM], double epsilon)
{
    /*calculate force on each particle*/

    double dist_sq, khat, fij;
    int i, j, k; //loop variables

    for(int i = 0; i < N; i++)
        for(int k = 0; k < DIM; k++)
            force[i][k] = 0;

    for(i = 0; i < N; i++)
    {
        for(j = i + 1; j < N; j++)
        {
            //calculate distance between i and j
            dist_sq = 0;
            for(k = 0; k < DIM; k++)
                dist_sq += (particle[i][1 + k] - particle[j][1 + k])*(particle[i][1 + k] - particle[j][1 + k]);

            //calculate force between i and j
            for(k = 0; k < DIM; k++)
            {
                khat = (particle[i][1 + k] - particle[j][1 + k]) / sqrt(dist_sq + epsilon * epsilon);
                fij = -particle[i][0] * particle[j][0] * khat / (dist_sq + epsilon * epsilon);
                force[i][k] += fij;
                force[j][k] -= fij;
            }
        }
    }
}
#endif
