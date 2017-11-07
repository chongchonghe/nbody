//
//  pp.h
//  gravity
//
//  Created by ChongChong He on 11/5/17.
//  Copyright © 2017 ChongChong He. All rights reserved.
//

#ifndef pp_h
#define pp_h

void acce(double **x, double *m, double **a, int N) {
    // x.size() = (N, 3), a.size() = (N, 3), m.size() = N
    assert(x != NULL);
    assert(m != NULL);
    assert(a != NULL);
    double r;
    double G = 1.0;
    int i, j, k;
    double epsilon = 1.0 / pow(N, 1.0/N);
    // int count = 0;
    
    for (i=0; i<N; i++) {
        for (k=0; k<3; k++)
            a[i][k] = 0.0;
        for (j=0; j<N; j++) {
            if (j != i) {
                r = 0.0;
                for (k=0; k<3; k++)
                    r += pow(x[j][k] - x[i][k], 2);
                r = sqrt(r);
                for (k=0; k<3; k++) {
                    a[i][k] += - G * m[j] * (x[j][k] - x[i][k]) /
                    pow(r * r + epsilon * epsilon, 3.0/2);
                    // count++;
                }
            }
        }
        for (k=0; k<3; k++)
            a[i][k] /= m[i];
    }
    // if (TEST)
    //   cout << "num of count: " << count << endl;
}

#endif /* pp_h */
