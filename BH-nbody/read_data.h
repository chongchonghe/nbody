#ifndef read_data_h
#define read_data_h

//extern int DIM;
#include "definitions.h"

void read_data(int N, DATA *data, char *file_name)
{
    /*read data from file_name
    format: m x y z xdot ydot zdot*/
    int i, k;

    FILE *fp;
	fp = fopen(file_name, "r");
    if(fp == NULL)
        exit(-1);

    for(i = 0; i < N; i++)
    {
        //format: m x y z xdot ydot zdot
        data[i].idx = i;
        fscanf(fp, "%lf", &data[i].mass);
        for(k = 0; k < DIM; k++)
            fscanf(fp, "%lf", &data[i].pos[k]);
        for(k = 0; k < DIM; k++)
            fscanf(fp, "%lf", &data[i].vel[k]);
    }
    fclose(fp);
}


#endif
