//
//  main.cpp
//  nbody-cpp
//
//  Usage: ./main N epsilon t_step n_steps op_freq out_dir [[start_id] or [ini_file_name]]

//  Created by ChongChong He on 12/19/17.
//  Copyright © 2017 ChongChong He. All rights reserved.

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>
#include <string>
#include <cassert>
#include "sys/stat.h"

//#include "definitions.h"
#include "def.h"
#include "read_data.h"
#include "save_data.h"
#include "calc_force.h"
#include "LF.h"

#define INTEG LF

#define UPDATE

#ifndef UPDATE
#define ISUPDATE 0
#else
#define ISUPDATE 1
#endif

using namespace std;

int main(int argc, char *argv[])
{
    //name of initial conditions file
    string file_name = "./test.txt";
    string outputdir = "./test/";  // dir of output files
    int N = 3;   //number of particles
    double epsilon = 0;  //softening parameter
    double t_step = 0.05;  //time step
    int n_steps = 2;   // number of steps
    int op_freq = 1;    // output frequency
//    string integrator = "LF2";  //ODE integration method
    int i; //loop variable
    int id = 0;
    bool isUpdate = true;
    
    if (argc == 8) {
        //check user input
        
        for (i=0; i<strlen(argv[1]); i++)
            assert(isdigit(argv[1][i]));
        N = atoi(argv[3]);
        
        epsilon = atof(argv[2]);
        assert(epsilon >= 0.0);
        
        t_step = atof(argv[3]);
        assert(t_step >= 1e-8);
        
        for (i=0; i<strlen(argv[4]); i++)
            assert(isdigit(argv[4][i]));
        n_steps = atoi(argv[4]);

        for (i=0; i<strlen(argv[5]); i++)
            assert(isdigit(argv[5][i]));
        op_freq = atof(argv[5]);
        
        outputdir = argv[6];
        
        for (i=0; i<strlen(argv[7]); i++)
            if (!isdigit(argv[7][i]))
                isUpdate = false;
        if (!isUpdate)
            file_name = argv[7];
        else {
            id = atoi(argv[7]);
            char suffix[20];
            snprintf(suffix, sizeof(suffix), "/data_%05d.txt", id);
            file_name = outputdir + suffix;
        }
    }
    
    cout << "filename = " << file_name << endl;
//    return 0;
    
//    DATA data[N];
//    double mass[N];
//    double position[N][DIM];
//    double velocity[N][DIM];
    double force[N][DIM];
    double particle[N][LEN];
    
    try {
        read_data(N, particle, file_name);  //read data from initial conditions file
    } catch (exception &ex) {
        cerr << "Exiting, because: " << ex.what() << "!\n";
        exit(EXIT_FAILURE);
    }
    
    try {
        save_data(N, particle, id++, outputdir);
    } catch (exception &ex) {
        cerr << "Exiting, because: " << ex.what() << "!\n";
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < n_steps; i++)
    {
        if ((i!=0) && (i % op_freq == 0))
//            save_data(N, particle, (int)(i/op_freq), outputdir);
            save_data(N, particle, id++, outputdir);

//        /*calculate the force on each particle*/
//        calc_force(N, data, force, epsilon);
//
//        INTEG(N, data, force, epsilon, t_step);
        INTEG(N, particle, force, t_step, epsilon);
    }

    return 0;
}
