//
//  integ_LF2.h
//  pp-nbody2
//
//  Created by ChongChong He on 11/14/17.
//  Copyright © 2017 ChongChong He. All rights reserved.
//

#ifndef integ_LF2_h
#define integ_LF2_h

extern int DIM;  //number of dimensions

void integ_LF2(double *mass, double **position, double **velocity, double **force, int N, double step, double epsilon)
{
    /*update position and velocity using leapfrog*/
    
    int i, k; //loop vars
    
    //calculate position after half a step

    double **pos_12;
    pos_12 = (double **) malloc(N * sizeof(double *));
    for (i = 0; i < N; i++) {
        pos_12[i] = (double *) malloc(DIM * sizeof(double));
        assert(pos_12[i]);
    }
    
    for(i = 0; i < N; i++)
        for(k = 0; k < DIM; k++)
            pos_12[i][k] = position[i][k] + 0.5 * step * velocity[i][k];
    
    //force at pos_12
    calc_force(mass, pos_12, force, N, epsilon);
    
    //update velocity and position
    for(i = 0; i < N; i++)
    {
        for(k = 0; k < DIM; k++)
        {
            velocity[i][k] += step * force[i][k] / mass[i];
            position[i][k] = pos_12[i][k] + 0.5 * step * velocity[i][k];
        }
    }
}

#endif

