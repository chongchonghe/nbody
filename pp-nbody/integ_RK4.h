/*******************
Mohammed Khalil and ChongChong He

integ Rk4.h   updates position and velocity using 4th order Runge-Kutta
********************/

#ifndef integ_RK4_h
#define integ_RK4_h

extern int DIM;  //number of dimensions

void integ_RK4(double mass[], double position[][DIM], double velocity[][DIM], double force[][DIM], int N, double step, double epsilon)
{
    /*update position and velocity using 4th order Runge-Kutta*/

    int i, k; //loop vars

    double k1_pos[N][DIM], k2_pos[N][DIM], k3_pos[N][DIM], k4_pos[N][DIM];
    double k1_vel[N][DIM], k2_vel[N][DIM], k3_vel[N][DIM], k4_vel[N][DIM];
    double pos_0[N][DIM], vel_0[N][DIM];

    for(i = 0; i < N; i++)
        for(k = 0; k < DIM; k++)
        {
            pos_0[i][k] = position[i][k];
            vel_0[i][k] = velocity[i][k];

            k1_pos[i][k] = step * velocity[i][k];
            k1_vel[i][k] = step * force[i][k] / mass[i];

            position[i][k] += k1_pos[i][k] / 2;
            velocity[i][k] += k1_vel[i][k] / 2;
        }

    calc_force(mass, position, force, N, epsilon); //force at k1

    for(i = 0; i < N; i++)
        for(k = 0; k < DIM; k++)
        {
            k2_pos[i][k] = step * velocity[i][k];
            k2_vel[i][k] = step * force[i][k] / mass[i];

            position[i][k] = pos_0[i][k] + k2_pos[i][k] / 2;
            velocity[i][k] = vel_0[i][k] + k2_vel[i][k] / 2;
        }

    calc_force(mass, position, force, N, epsilon); //force at k2

    for(i = 0; i < N; i++)
        for(k = 0; k < DIM; k++)
        {
            k3_pos[i][k] = step * velocity[i][k];
            k3_vel[i][k] = step * force[i][k] / mass[i];

            position[i][k] = pos_0[i][k] + k3_pos[i][k];
            velocity[i][k] = vel_0[i][k] + k3_vel[i][k];
        }

    calc_force(mass, position, force, N, epsilon); //force at k3

    for(i = 0; i < N; i++)
        for(k = 0; k < DIM; k++)
        {
            k4_pos[i][k] = step * velocity[i][k];
            k4_vel[i][k] = step * force[i][k] / mass[i];

            position[i][k] = pos_0[i][k] + k1_pos[i][k] / 6 + k2_pos[i][k] / 3 + k3_pos[i][k] / 3 + k4_pos[i][k] / 6;
            velocity[i][k] = vel_0[i][k] + k1_vel[i][k] / 6 + k2_vel[i][k] / 3 + k3_vel[i][k] / 3 + k4_vel[i][k] / 6;
        }
}

#endif
