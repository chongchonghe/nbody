//
//  save_data.h
//  pp-nbody2
//
//  Created by ChongChong He on 11/14/17.
//  Copyright © 2017 ChongChong He. All rights reserved.
//

#ifndef save_data_h
#define save_data_h

extern int DIM;  //number of dimensions

void save_data(double *mass, double **position, double **velocity, int N, int file_num)
{
    //create data directory
    struct stat st = {0};
    if (stat("./data/", &st) == -1) {
        mkdir("./data/", 0700);
    }
    
    char s_number[4];
    sprintf(s_number, "%d", file_num);  //convert file_num to string
    
    char filename[40];
    snprintf(filename, sizeof(filename), "./data/data_%04d.txt", file_num);
    
    FILE *fp; //File to write positions to
    fp = fopen(filename, "w");
    if(fp == NULL)
        exit(-1);
    
    for(int i = 0; i < N; i++)
    {
        //format m x y z xdot ydot zdot
        fprintf(fp, "%f ", mass[i]);
        for(int k = 0; k < DIM; k++)
            fprintf(fp, "%f ", position[i][k]);
        for(int k = 0; k < DIM; k++)
            fprintf(fp, "%f ", velocity[i][k]);
        fprintf(fp, "\n");
    }
    
    fclose(fp);
}

#endif
