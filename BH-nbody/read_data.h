#ifndef read_data_h
#define read_data_h

extern int DIM;

void read_data(float mass[], float position[][DIM], float velocity[][DIM], int N, char *file_name)
{
    int i, k;

    FILE *fp;

	fp = fopen(file_name, "r");
    if(fp == NULL)
        exit(-1);

    for(i = 0; i < N; i++)
    {
        //format: m x y z xdot ydot zdot
        fscanf(fp, "%f", &mass[i]);
        for(k = 0; k < DIM; k++)
            fscanf(fp, "%f", &position[i][k]);
        for(k = 0; k < DIM; k++)
            fscanf(fp, "%f", &velocity[i][k]);
    }
    fclose(fp);
}


#endif
