/*************************
Mohammed Khalil and ChongChong He

read_data.h   reads data from initial conditions file
**************************/

#ifndef read_data_h
#define read_data_h

//#include "definitions.h"
#include "def.h"
//extern int DIM;
//extern int LEN;

void read_data(int N, double particle[][LEN], std::string file_name)
//void read_data(int N, DATA *data, std::string file_name)
{
    /*read data from file_name
    format: m x y z xdot ydot zdot*/
    int i, k;

    std::ifstream infile;
    infile.open(file_name);
    
    if (!infile.is_open())
        throw std::runtime_error("Cannot open file");

    for(i = 0; i < N; i++)
    {
        //format: m x y z xdot ydot zdot
//        data[i].idx = i;
        infile >> particle[i][0];
//        infile >> data[i].mass;
        for (k = 0; k < DIM; k++)
            infile >> particle[i][1 + k];
//            infile >> data[i].pos[k];
        for (k = 0; k < DIM; k++)
            infile >> particle[i][1 + DIM + k];
//            infile >> data[i].vel[k];
    }
    
    infile.close();
}

#endif
