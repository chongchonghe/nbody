/*******************
Mohammed Khalil and ChongChong He

integ_LF2.h updates position and velocity using 2nd order leapfrog
********************/

#ifndef integ_LF2_h
#define integ_LF2_h

extern int DIM;  //number of dimensions

void integ_LF2(double mass[], double position[][DIM], double velocity[][DIM], double force[][DIM], int N, double step, double epsilon)
{
    /*update position and velocity using 2nd order leapfrog*/

    int i, k; //loop vars

    //advance position by half a step
    for(i = 0; i < N; i++)
        for(k = 0; k < DIM; k++)
            position[i][k] += 0.5 * step * velocity[i][k];

    //force after half a step
    calc_force(mass, position, force, N, epsilon);

    //update velocity and position
    for(i = 0; i < N; i++)
    {
        for(k = 0; k < DIM; k++)
        {
            velocity[i][k] += step * force[i][k] / mass[i];
            position[i][k] += 0.5 * step * velocity[i][k];
        }
    }
}

#endif
