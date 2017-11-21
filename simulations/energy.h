/*
  energy.h -- Calculate the kinetic and potential energy of all particles from a data file
*/

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
