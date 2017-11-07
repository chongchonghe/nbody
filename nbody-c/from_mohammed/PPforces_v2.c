#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

const int DIM = 3;  //number of dimensions

void initialize(float position[][DIM], float velocity[][DIM], float force[][DIM], int N, float length)
{
    //fill position[][] with random positions; N is number of particles; length is side length of cube
    srand(time(NULL));  //seed srand() using Unix time
    for(int i = 0; i < N; i++)
        for(int k = 0; k < DIM; k++)
        {
            position[i][k] = length * ((float)rand() / (float)RAND_MAX);
            velocity[i][k] = 0;
        }
    /*
    //three bodies
    position[0][0] = 0; position[0][1] = -1;  position[0][2] = 0;
    position[1][0] = -0.86602540; position[1][1] = 0.5;  position[1][2] = 0;
    position[2][0] = 0.86602540; position[2][1] = 0.5;  position[2][2] = 0;
    velocity[0][0] = 1; velocity[0][1] = 0;  velocity[0][2] = 0;
    velocity[1][0] = -0.5; velocity[1][1] = -0.86602540;  velocity[1][2] = 0;
    velocity[2][0] = -0.5; velocity[2][1] = 0.86602540;  velocity[2][2] = 0;
    */
}

void calc_force(float position[][DIM], float force[][DIM], int N, float epsilon)
{
    float dist_sq, khat, fij;
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
                dist_sq += (position[i][k] - position[j][k])*(position[i][k] - position[j][k]);

            //calculate force between i and j
            for(k = 0; k < DIM; k++)
            {
                khat = (position[i][k] - position[j][k]) / sqrt(dist_sq);
                fij = -khat / (dist_sq + epsilon*epsilon);
                force[i][k] += fij;
                force[j][k] -= fij;
            }
        }
    }
}

void update_pos(float position[][DIM], float velocity[][DIM], float force[][DIM], int N, float step)
{
    //update position and velocity using leapfrog
    float kint;
    int i, k; //loop vars

    for(i = 0; i < N; i++)
    {
        for(k = 0; k < DIM; k++)
        {
            kint = position[i][k] + 0.5 * step * velocity[i][k];
            velocity[i][k] += step * force[i][k];   //divide by mass if not 1
            position[i][k] = kint + 0.5 * step * velocity[i][k];
        }
    }
}

void save_pos(float position[][DIM], int N)
{
	FILE *fp; //File to write positions to
	fp = fopen("positions.txt", "a");
    if(fp == NULL)
	    exit(-1);

    for(int i = 0; i < N; i++)
    {
        for(int k = 0; k < DIM; k++)
            fprintf(fp, "%f ", position[i][k]);
        fprintf(fp, "\n");
    }

    fclose(fp);
}

int main()
{
    int N = 100;
    float length = 10;   //cube side length
    float epsilon = 0.1*length / pow(N, 1/3);  //softening parameter
    float step = 0.01;    //time step
    float sim_time = 100;  //simulation time

    float position[N][DIM];
    float velocity[N][DIM];
    float force[N][DIM];

    initialize(position, velocity, force, N, length);  //fill random positions and set force to zero
    //save_pos(position, N);
    for(int i = 0; i < (int)(sim_time/step); i++)
    {
        calc_force(position, force, N, epsilon);
        update_pos(position, velocity, force, N, step);
        if(i % 100 == 0)
            save_pos(position, N);
    }

    return 0;
}

