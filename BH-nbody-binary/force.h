#ifndef force_h
#define force_h

#include "definitions.h"

void direct_force_cell(const DATA *point, const NODE *node, double *force, double epsilon)
{
    /*calculate force between data point and the cog of node*/
    int i;
    double ihat;

    double dist_sq = 0;
    for(i = 0; i < DIM; i++)
        dist_sq += (point->pos[i] - node->cog[i]) * (point->pos[i] - node->cog[i]);

    if (dist_sq != 0)
        for(i = 0; i < DIM; i++){
            ihat = (point->pos[i] - node->cog[i]) / sqrt(dist_sq + epsilon*epsilon);
            force[i] -= point->mass * node->mass * ihat / (dist_sq + epsilon*epsilon);;
        }
}

void direct_force_leaf(const DATA *point, const DATA *leaf, double *force, double epsilon)
{
    /*calculate force between data point and leaf*/
    int i;
    double ihat;

    double dist_sq = 0;
    for(i = 0; i < DIM; i++)
        dist_sq += (point->pos[i] - leaf->pos[i]) * (point->pos[i] - leaf->pos[i]);

    if (dist_sq != 0)
        for(i = 0; i < DIM; i++){
            ihat = (point->pos[i] - leaf->pos[i]) / sqrt(dist_sq + epsilon*epsilon);
            force[i] -= point->mass * leaf->mass * ihat / (dist_sq + epsilon*epsilon);;
        }
}

int add_to_force(const DATA *point, const NODE *node, double *force, double epsilon)
{
    /*tree walk to calculate forces*/
    int i,k;
    double cell_size, theta;

    double dist_sq = 0;
    for(i = 0; i < DIM; i++)
        dist_sq += (point->pos[i] - node->cog[i]) * (point->pos[i] - node->cog[i]);
    if(dist_sq == 0) return 0;

    cell_size = ((node->cmax[0] - node->cmin[0]) +
                 (node->cmax[1] - node->cmin[1]) +
                 (node->cmax[2] - node->cmin[2])) / 3;

    theta = cell_size / sqrt(dist_sq);

    if (theta < THETA_CRIT)
        direct_force_cell(point, node, force, epsilon);
    else
        for (i = 0; i < CELLS_PER_NODE; i++)
            if (node->cell[i] != NULL) //is cell a node?
                add_to_force(point, node->cell[i], force, epsilon);
            else if (node->leaf[i] != NULL )  //is cell a leaf?
                direct_force_leaf(point, node->leaf[i], force, epsilon);
    return 0;
}

void calc_force(int N, const DATA *data, const NODE *root, double force[][DIM], double epsilon)
{
    /*set force to zero and pass data point by point to add_to_force()*/
    int j,k;

    for(j = 0; j < N; j++)
        for(k = 0; k < DIM; k++)
            force[j][k] = 0;   // set all forces to zero

    for(j = 0; j < N; j++) //loop over particles
        add_to_force(&data[j], root, force[j], epsilon);
}

#endif
