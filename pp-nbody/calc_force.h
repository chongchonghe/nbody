#ifndef calc_force_h
#define calc_force_h

extern int DIM;

void calc_force(double mass[], double position[][DIM], double force[][DIM], int N, double epsilon)
{
    /*calculate force on each particle*/

    double dist_sq, khat, fij;
    int i, j, k; //loop variables

    for(int i = 0; i < N; i++)
        for(int k = 0; k < DIM; k++)
            force[i][k] = 0;

    for(i = 0; i < N; i++)
    {
        for(j = i + 1; j < N; j++)
        {
            //calculate distance between i and j
            dist_sq = 0;
            for(k = 0; k < DIM; k++)
                dist_sq += (position[i][k] - position[j][k])*(position[i][k] - position[j][k]);

            //calculate force between i and j
            for(k = 0; k < DIM; k++)
            {
                khat = (position[i][k] - position[j][k]) / sqrt(dist_sq + epsilon * epsilon);
                fij = -mass[i] * mass[j] * khat / (dist_sq + epsilon*epsilon);
                force[i][k] += fij;
                force[j][k] -= fij;
            }
        }
    }
}
#endif
