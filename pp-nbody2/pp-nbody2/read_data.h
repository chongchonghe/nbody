//
//  read_data.h
//  pp-nbody2
//
//  Created by ChongChong He on 11/14/17.
//  Copyright © 2017 ChongChong He. All rights reserved.
//

#ifndef read_data_h
#define read_data_h

extern int DIM;

//void read_data(double mass[], double position[][DIM], double velocity[][DIM], int N, char *file_name)
void read_data(double *mass, double **position, double **velocity, int N, const char *file_name)
{
    int i, k;

    FILE *fp;

    fp = fopen(file_name, "r");
    if(fp == NULL)
    exit(-1);

    for(i = 0; i < N; i++)
    {
        //format: m x y z xdot ydot zdot
        fscanf(fp, "%lf", &mass[i]);
        for(k = 0; k < DIM; k++)
            fscanf(fp, "%lf", &position[i][k]);
            for(k = 0; k < DIM; k++)
                fscanf(fp, "%lf", &velocity[i][k]);
                }
    fclose(fp);
}

#endif /* read_data_h */
