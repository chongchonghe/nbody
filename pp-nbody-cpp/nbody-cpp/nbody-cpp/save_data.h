/*************************
Mohammed Khalil and ChongChong He

save_data.h   saves data to output directory
**************************/

#ifndef save_data_h
#define save_data_h

//#include "definitions.h"
#include "def.h"

//void save_data(int N, const DATA *data, int file_num, std::string outputdir)
void save_data(int N, double particle[][LEN], int file_num, std::string outputdir)
{
    /*save data to outputdir
    format: m x y z xdot ydot zdot*/

    //format file name
    char suffix[16];
    snprintf(suffix, sizeof(suffix), "/data_%05d.txt", file_num);
    std::string filename = outputdir + suffix;
    
    std::ofstream outfile;
    outfile.open(filename);
    if (!outfile)
        throw std::runtime_error("Cannot create file. Perhaps directory not exit");
    outfile.precision(15);

    for(int i = 0; i < N; i++)
    {
        //format m x y z xdot ydot zdot
        outfile << particle[i][0] << " ";
        for(int k = 0; k < DIM; k++)
            outfile << particle[i][1 + k] << " ";
        for(int k = 0; k < DIM; k++)
            outfile << particle[i][1 + DIM + k] << " ";
        outfile << "\n";
    }

    outfile.close();

}

#endif
