#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

const int DIM = 3;  //number of dimensions

int main()
{
    //three bodies in elliptical orbits
    int N = 3;
    int i, j, k; //loop variables

    float mass[3] = {1, 1, 1};
    float position[3][3] = { {0, -1, 0}, {-0.86602540, 0.5, 0}, {0.86602540, 0.5, 0} };
    float velocity[3][3] = { {1, 0, 0}, {-0.5, -0.86602540, 0}, {-.5, 0.86602540, 0} };

    FILE *fp; //File to write initial masses, positions, and velocities
	fp = fopen("data000.txt", "w");
    if(fp == NULL)
	    exit(-1);

    for(i = 0; i < N; i++)
    {
        //format m x y z xdot ydot zdot
        fprintf(fp, "%f ", mass[i]);
        for(k = 0; k < DIM; k++)
            fprintf(fp, "%f ", position[i][k]);
        for(k = 0; k < DIM; k++)
            fprintf(fp, "%f ", velocity[i][k]);
        fprintf(fp, "\n");
    }

    fclose(fp);

    /*
    //fill random positions with zero velocities; N is number of particles; length is side length of cube
    srand(time(NULL));  //seed srand() using Unix time
    for(int i = 0; i < N; i++)
    {
        mass[i] = 1;
        for(int k = 0; k < DIM; k++)
        {
            position[i][k] = length * ((float)rand() / (float)RAND_MAX);
            velocity[i][k] = 0;
        }
    }
    */

    return 0;
}


