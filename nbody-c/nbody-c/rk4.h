/*
  rk4.h
  Description: Runge-Kutta 4th-order solver
  Author: ChongChong He 
  Created on 10/27/17.
*/

#ifndef rk4_h
#define rk4_h

void rk4(int n, double *y, const double t, const double h,
         void deriv(int, double, double *, double *))
{
    assert(y != NULL);
    double *yt, *dydt1, *dydt2, *dydt3, *dydt4;
    yt = (double *) malloc(n * sizeof(double));
    dydt1 = (double *) malloc(n * sizeof(double));
    dydt2 = (double *) malloc(n * sizeof(double));
    dydt3 = (double *) malloc(n * sizeof(double));
    dydt4 = (double *) malloc(n * sizeof(double));
    int i;
    
    deriv(n, t, y, dydt1);
    for (i = 0; i < n; i++)
        yt[i] = y[i] + 0.5 * h * dydt1[i];
    deriv(n, t, yt, dydt2);
    for (i = 0; i < n; i++)
        yt[i] = y[i] + 0.5 * h * dydt2[i];
    deriv(n, t, yt, dydt3);
    for (i = 0; i < n; i++)
        yt[i] = y[i] + h * dydt3[i];
    deriv(n, t, yt, dydt4);
    for (i = 0; i < n; i++) {
        y[i] += dydt1[i] * h / 6 + dydt2[i] * h / 3 +
        dydt3[i] * h / 3 + dydt4[i] * h / 6;
    }
    
    free((void *) yt);
    free((void *) dydt1);
    free((void *) dydt2);
    free((void *) dydt3);
    free((void *) dydt4);
}

#endif /* rk4_h */
