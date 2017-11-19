/*
  readenergy.c -- Calculate the kinetic and potential energy of all particles from a data file
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "BH-nbody/read_data.h"

double cal_potential(int N, DATA *data)
{
    double pot = 0.0;
    double dis2;
    for (int i = 0; i < N; i++) {
	for (int j = i + 1; j < N; j++) {
	    dis2 = 0.0;
	    for (int k = 0; k < DIM; k++)
		dis2 += pow((data[i].pos[k] - data[j].pos[k]), 2.0);
	    pot += - data[i].mass * data[j].mass / sqrt(dis2);
	}
    }
    return pot;
}

double cal_kinetic(int N, DATA *data)
{
    double kin = 0.0;
    for (int i = 0; i < N; i++)
	for (int k = 0; k < DIM; k ++)
	    kin += data[i].mass * data[i].vel[k] * data[i].vel[k];
    return kin / 2.0;
}


int main(int argc, char *argv[])
    {
	if (argc != 2) {
	    printf("usage: ./readenergy data/path\n");
	    return -1;
	}
	
	char *file_name = argv[1];
	int N = 2;
	DATA data[N];
	double k, w;
	read_data(N, data, file_name);
	printf("Kinetic energy:\n");
	k = cal_kinetic(N, data);
	printf("%lf\n", k);
	printf("Potential energy:\n");
	w = cal_potential(N, data);
	printf("%lf\n", w);
	printf("Virial ratio:\n");
	printf("%lf\n", -k / w);
	return 0;
    }
