//
//  is_binary.h
//  nbody-xcode
//
//  Created by ChongChong He on 11/19/17.
//  Copyright © 2017 ChongChong He. All rights reserved.
//

#ifndef is_binary_h
#define is_binary_h

_Bool is_binary(double dClose, double m1, double m2, double position1[DIM],
                double position2[DIM], double velocity1[DIM], double velocity2[DIM])
{
    // check distance < alpha * dClose
    double beta = 2.0;
    double distSq = 0.0;
    
    for (int i = 0; i < DIM; i++)
        distSq += (position1[i] - position2[i]) * (position1[i] - position2[i]);
    if (distSq > dClose * dClose)
        return 0;
    
    // if close enough to each other, check K / |W| < 1 - beta^-1
    double reducedMass;
    double relaVelSq = 0.0;
    double kinetic;
    double potentialAbs;
    
    for (int i = 0; i < DIM; i++)
        relaVelSq += (velocity1[i] - velocity2[i]) * (velocity1[i] - velocity2[i]);
    reducedMass = m1 * m2 / (m1 + m2);
    kinetic = 0.5 * reducedMass * relaVelSq;
    potentialAbs = m1 * m2 / sqrt(distSq);
    return (kinetic / potentialAbs < 1.0 - 1.0 / beta);
}

#endif /* is_binary_h */
