#ifndef integ_LF2_h
#define integ_LF2_h

extern int DIM;  //number of dimensions

void integ_LF2(float mass[], float position[][DIM], float velocity[][DIM], float force[][DIM], int N, float step)
{
    //update position and velocity using leapfrog
    float kint;
    int i, k; //loop vars

    for(i = 0; i < N; i++)
    {
        for(k = 0; k < DIM; k++)
        {
            kint = position[i][k] + 0.5 * step * velocity[i][k];
            velocity[i][k] += step * force[i][k]/mass[i];   //divide by mass if not 1
            position[i][k] = kint + 0.5 * step * velocity[i][k];
        }
    }
}

#endif
