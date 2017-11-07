/*
  leapfrog.h
  Description: leapfrog drift-kick-drift method for nbody simulation
  Author: ChongChong He
  Created on 11/5/17.
*/

#ifndef leap_h
#define leap_h

void lfnbody(int n, double *y, const double t, const double h,
         void deriv(int, double, double *, double *))
{
    const int dim = 3;
    int column = 2 * dim + 1;
    int N = n / column;
    
    assert(y != NULL);
    
    double *yt, *dydt;
    yt = (double *) malloc(n * sizeof(double));
    dydt = (double *) malloc(n * sizeof(double));
    
    int i, j;
    
    for (i = 0; i < N; i++)
        for (j = 1; j < dim + 1; j++)
            y[column * i + j] += 0.5 * h * y[column * i + dim + j];
    
    deriv(n, t, y, dydt);
    
//    for (i=0; i<n; i+=2)
//        y[i+1] = y[i+1] + h * dydt[i+1];
    
    for (i = 0; i < N; i++)
        for (j = 1; j < dim + 1; j++)
            y[column * i + dim + j] += h * dydt[column * i + dim + j];
    
//    for (i=0; i<n; i+=2)
//        y[i] = y[i] + 0.5 * h * y[i+1];
    
    for (i=0; i < N; i++)
        for (j = 1; j < dim + 1; j++)
            y[column * i + j] += 0.5 * h * y[column * i + dim + j];
    
    free((void *) yt);
    free((void *) dydt);
}

#endif /* leap_h */
