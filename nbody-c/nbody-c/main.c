//
//  main.c
//  nbody-c
//
//  Created by ChongChong He on 11/5/17.
//  Copyright © 2017 ChongChong He. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "rk4.h"
#include "lfnbody.h"
#include "pp.h"
#include "myarray.h"

/* Uncomment the following line to do test. USE SMALL N */
#define TEST 1

#ifndef TEST
#define TEST 0
#endif

/* Uncomment the following line to diable DEBUG */
// #define NDEBUG

void deriv(int n, double t, double *y, double *dydt) {
    // y.size() = 7n, m.size() = n, dydt.size() = 7n
    int i, j;
    int N = n / 7;
    double *m;
    m = (double *) malloc(N * sizeof(double));
    double **x, **a;
    x = (double **) malloc(N * sizeof(double *));
    a = (double **) malloc(N * sizeof(double *));
    assert(x);
    assert(a);
    for (i = 0; i < N; i++) {
        x[i] = (double *) malloc(3 * sizeof(double));
        a[i] = (double *) malloc(3 * sizeof(double));
        assert(x[i]);
        assert(a[i]);
    }

    // do the deriv of positions and mass
    for (i = 0; i < N; i++) {
        dydt[7 * i] = 0.0;
        for (j = 0; j < 3; j++) {
            dydt[7 * i + j + 1] = y[7 * i + j + 4];
        }
    }
    
    // write x from y. Later should merge with the block above
    for (i = 0; i < N; i++) {
        m[i] = y[7 * i];
        for (j = 0; j < 3; j++) {
            x[i][j] = y[7 * i + j + 1];
        }
    }
    
    acce(x, m, a, N);
    
    // write back to dydt
    for (i = 0; i < N; i++) {
        for (j = 0; j < 3; j++) {
            dydt[7 * i + j + 4] = a[i][j];
        }
    }
    
    for (i = 0; i < N; i++) {
        free((void *) x[i]);
        free((void *) a[i]);
    }
    free((void *) x);
    free((void *) a);
}

void inity(double *y, const int N) {
    //    Initialize the array x with random numbers
    //    y.size() = 7N, m.size() = N
    assert(y != NULL);
    for (int i = 0; i < N; i++){
        y[i * 7] = 1.0;
        for (int j = 0; j < 6; j++)
            y[i * 7 + j + 1] = drand48();
    }
}

int main(int argc, const char * argv[]) {

    assert(argc >= 2);
    // int N = pow(2, strtol(argv[1], NULL, 10));
    int N = strtol(argv[1], NULL, 10);
    assert(N >= 1);
    int n = N * 7;
    
    srand48(getpid());
    
    if (TEST)
        printf("\nN = %d\n", N);
    
    double t, h;
    t = 0.0;
    h = 0.1;
    
    double *y, *dydt;
    y = (double *) malloc(n * sizeof(double));
    dydt = (double *) malloc(n * sizeof(double));
    
    inity(y, N);
    
    if (TEST) {
        printf("Initial values:\n");
        printf("y = \n");
        printArrayPtr(y, N, 7);

    	deriv(n, t, y, dydt);
        printf("dydt = \n");
        printArrayPtr(dydt, N, 7);
    }
    
    //    rk4(n, y, t, h, deriv);
    lfnbody(n, y, t, h, deriv);

    if (TEST) {
        printf("after 1 step with h=0.1:\n");
        printf("y = \n");
        printArrayPtr(y, N, 7);
    }
    
    free((void *) y);
    free((void *) dydt);
    
    return 0;
}
