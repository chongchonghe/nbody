/*
  readenergy.c -- Calculate the kinetic and potential energy of all particles from a data file
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "BH-nbody/read_data.h"
#include "energy.h"

int main(int argc, char *argv[])
    {
	if (argc != 3) {
	    printf("usage: ./readenergy data/path N\n");
	    return -1;
	}
	
	char *file_name = argv[1];
	int N;

        for (int i=0; i<strlen(argv[2]); i++)
            assert(isdigit(argv[2][i]));
        N = atoi(argv[2]);

	DATA data[N];
	double k, w;
	read_data(N, data, file_name);
	printf("Kinetic energy:\n");
	k = cal_kinetic(N, data);
	printf("%lf\n", k);
	printf("Potential energy:\n");
	w = cal_potential(N, data);
	printf("%lf\n", w);
	printf("Total energy:\n");
	printf("%lf\n", w + k);
	printf("Virial ratio:\n");
	printf("%lf\n", -k / w);
	return 0;
    }
