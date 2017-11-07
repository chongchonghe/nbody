//
//  myarray.h
//  gravity
//
//  Created by ChongChong He on 11/5/17.
//  Copyright © 2017 ChongChong He. All rights reserved.
//

#ifndef myarray_h
#define myarray_h

void printArrayPtr(double *arr, int N, int M) {
    assert(arr != NULL);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
            printf("%f\t", arr[7 * i + j]);
        printf("\n");
    }
    printf("\n");
}

void init(double **x, double *m, const int n) {
    //    Initialize the array x with random numbers
    assert(x != NULL);
    assert(m != NULL);
    int i, j;
    for (j=0; j<n; j++){
        m[j] = 1.0;
        for (i=0; i<3; i++) {
            //            x[j][i] = drand48();
            x[j][i] = 10*j + i;
        }
    }
}

#endif /* myarray_h */
