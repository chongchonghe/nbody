#ifndef calc_PP_force_h
#define calc_PP_force_h

//extern int DIM;

void calc_PP_force(int N, DATA *data, double force[][DIM], double epsilon)
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
                dist_sq += (data[i].pos[k] - data[j].pos[k])*(data[i].pos[k] - data[j].pos[k]);

            //calculate force between i and j
            for(k = 0; k < DIM; k++)
            {
                khat = (data[i].pos[k] - data[j].pos[k]) / sqrt(dist_sq);
                fij = -data[i].mass * data[j].mass * khat / (dist_sq + epsilon*epsilon);
                force[i][k] += fij;
                force[j][k] -= fij;
            }
        }
    }
}

#endif