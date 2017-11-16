#ifndef save_data_h
#define save_data_h

extern int DIM;  //number of dimensions

void save_data(double mass[], double position[][DIM], double velocity[][DIM], int N, int file_num, char *outputdir)
{
    /*save data to outputdir
    format: m x y z xdot ydot zdot*/

    //create data directory
    struct stat st = {0};
    if (stat(outputdir, &st) == -1) {
        mkdir(outputdir, 0700);
    }

    char filename[80];
    snprintf(filename, sizeof(filename), "%s/data_%04d.txt", outputdir, file_num);

    FILE *fp; //File to write positions to
    fp = fopen(filename, "w");
    if(fp == NULL)
	exit(-1);

    for(int i = 0; i < N; i++)
    {
        //format m x y z xdot ydot zdot
        fprintf(fp, "%lf ", mass[i]);
        for(int k = 0; k < DIM; k++)
            fprintf(fp, "%lf ", position[i][k]);
        for(int k = 0; k < DIM; k++)
            fprintf(fp, "%lf ", velocity[i][k]);
        fprintf(fp, "\n");
    }

    fclose(fp);
}

#endif
