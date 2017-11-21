/*******************
Mohammed Khalil and ChongChong He

read_data.h   reads data from initial conditions file
********************/

#ifndef read_data_h
#define read_data_h

extern int DIM;

void read_data(double mass[], double position[][DIM], double velocity[][DIM], int N, char *file_name)
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
        fscanf(fp, "%lf", &mass[i]);
        for(k = 0; k < DIM; k++)
            fscanf(fp, "%lf", &position[i][k]);
        for(k = 0; k < DIM; k++)
            fscanf(fp, "%lf", &velocity[i][k]);
    }
    fclose(fp);
}


#endif
