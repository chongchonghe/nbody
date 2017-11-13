/*
 ** bhtree.c -- DCR 10/25/17
 ** ========
 ** Demo of Barnes & Hut tree code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include "read_data.h"
#include "integ_LF2.h"
#include "integ_RK4.h"
#include "save_data.h"

/* particle definitions */

#define MAX_NUM_PART 5000

typedef struct {
  int idx;
  double mass;
  double pos[3];
  } DATA;

/* tree definitions */

#define CELLS_PER_NODE 8 /* oct-tree (3-D) */

struct node_s {
  double cmin[3], cmax[3], pos[3];
  double mass, cog[3];
  struct node_s *cell[CELLS_PER_NODE];
  const DATA *leaf[CELLS_PER_NODE];
  };

typedef struct node_s NODE;

/* functions follow */

void kill_node(NODE *node)
{
	/* call this to free memory */

	int i;

	assert(node != NULL);

	for (i = 0; i < CELLS_PER_NODE; i++)
		if (node->cell[i] != NULL)
			kill_node(node->cell[i]);

	free((void *) node);
	}

void make_node(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, NODE **node)
{
	/* allocates memory for node with given physical dimensions */

	int i;

	*node = (NODE *) malloc(sizeof(NODE));
	assert(*node != NULL);

	(*node)->cmin[0] = xmin;
	(*node)->cmax[0] = xmax;
	(*node)->cmin[1] = ymin;
	(*node)->cmax[1] = ymax;
	(*node)->cmin[2] = zmin;
	(*node)->cmax[2] = zmax;

	/* node position is at center of cell */

	(*node)->pos[0] = 0.5*((*node)->cmin[0] + (*node)->cmax[0]);
	(*node)->pos[1] = 0.5*((*node)->cmin[1] + (*node)->cmax[1]);
	(*node)->pos[2] = 0.5*((*node)->cmin[2] + (*node)->cmax[2]);

	/* initialize storage */

	for (i = 0; i < CELLS_PER_NODE; i++) {
		(*node)->cell[i] = NULL;
		(*node)->leaf[i] = NULL;
		}
	}

void add_to_tree(NODE *node, const DATA *data)
{
	/* inserts data into tree at node */

	int i, idx, idy, idz;

	/* determine index of cell to which particle belongs */

	idx = (data->pos[0] < node->pos[0] ? -1 : 1);
	idy = (data->pos[1] < node->pos[1] ? -1 : 1);
	idz = (data->pos[2] < node->pos[2] ? -1 : 1);

	i = (idx + 1) / 2 + (idy + 1 + 2 * (idz + 1));

	/* add to existing node, or make a new node, or make a new leaf */

	if (node->cell[i] != NULL)
		add_to_tree(node->cell[i], data);
	else if (node->leaf[i] != NULL) {
		make_node(idx < 0 ? node->cmin[0] : node->pos[0],
				  idx < 0 ? node->pos[0] : node->cmax[0],
				  idy < 0 ? node->cmin[1] : node->pos[1],
				  idy < 0 ? node->pos[1] : node->cmax[1],
				  idz < 0 ? node->cmin[2] : node->pos[2],
				  idz < 0 ? node->pos[2] : node->cmax[2],
				  &node->cell[i]);
		add_to_tree(node->cell[i], node->leaf[i]);
		add_to_tree(node->cell[i], data);
		node->leaf[i] = NULL;
		}
	else
		node->leaf[i] = data;
	}

void calc_moments(NODE *node)
{
	/* loops over cells of node to find enclosed mass and center of gravity */

	const DATA *leaf;
	NODE *cell;
	int i;

	assert(node != NULL);

	node->mass = node->cog[0] = node->cog[1] = node->cog[2] = 0.0;

	for (i = 0; i < CELLS_PER_NODE; i++)
		if (node->cell[i] != NULL) {
			cell = node->cell[i];
			calc_moments(cell);
			node->mass += cell->mass;
			node->cog[0] += cell->mass * cell->cog[0];
			node->cog[1] += cell->mass * cell->cog[1];
			node->cog[2] += cell->mass * cell->cog[2];
			}
		else if (node->leaf[i] != NULL) {
			leaf = node->leaf[i];
			node->mass += leaf->mass;
			node->cog[0] += leaf->mass * leaf->pos[0];
			node->cog[1] += leaf->mass * leaf->pos[1];
			node->cog[2] += leaf->mass * leaf->pos[2];
			}

	assert(node->mass > 0.0);

	node->cog[0] /= node->mass;
	node->cog[1] /= node->mass;
	node->cog[2] /= node->mass;
	}

void build_tree(int n, const DATA *data, NODE **root)
{
	/* construct the tree with the data (returns root node) */

	int i;
	double xmin, xmax, ymin, ymax, zmin, zmax, dx, dy, dz, dmin, dmax;

	xmin = ymin = zmin = HUGE_VAL;
	xmax = ymax = zmax = -HUGE_VAL;

	/* get bounds of cube that contains all particles */

	for (i = 0; i < n; i++) {
		if (data[i].pos[0] < xmin)
			xmin = data[i].pos[0];
		if (data[i].pos[0] > xmax)
			xmax = data[i].pos[0];
		if (data[i].pos[1] < ymin)
			ymin = data[i].pos[1];
		if (data[i].pos[1] > ymax)
			ymax = data[i].pos[1];
		if (data[i].pos[2] < zmin)
			zmin = data[i].pos[2];
		if (data[i].pos[2] > zmax)
			zmax = data[i].pos[2];
		}

	dx = xmax - xmin;
	dy = ymax - ymin;
	dz = zmax - zmin;

	assert(dx >= 0.0 && dy >= 0.0 && dz >= 0.0);

	/* get minimum corner */

	if (xmin < ymin && xmin < zmin)
		dmin = xmin;
	else if (ymin < zmin && ymin < xmin)
		dmin = ymin;
	else
		dmin = zmin;

	dmax = dmin;

	/* get maximum dimension */

	if (dx > dy && dx > dz)
		dmax += dx;
	else if (dy > dz && dy > dx)
		dmax += dy;
	else
		dmax += dz;

	if (dmin == dmax) {
		fprintf(stderr, "Degenerate tree: all particles occupy same point in space.\n");
		return;
		}

	make_node(dmin, dmax, dmin, dmax, dmin, dmax, root);

	/* fill tree with particles */

	for (i = 0; i < n; i++)
		add_to_tree(*root, &data[i]);

	/* get node gravity centers and masses */

	calc_moments(*root);
	}


void add_to_force(const DATA *point, NODE *node, double *force)
{
    /* data[i].pos[0]: x position of particle i
       point is DATA struct with pos[0,1,2] mass and idx
       node is NODE struct with cmin, cmax, mass, pos, cog, cell, leaf*/
    int i;
    double cell_size, theta;
    double theta_crit = 0.6;

    double dist_sq = 0;
    for(int i = 0; i < 3; i++)
        dist_sq += (point->pos[i] - node->cog[i]) * (point->pos[i] - node->cog[i]);

    cell_size = ((node->cmax[0] - node->cmin[0]) +
                 (node->cmax[1] - node->cmin[1]) +
                 (node->cmax[2] - node->cmin[2])) / 3;
    theta = cell_size / sqrt(dist_sq);

    if (theta < theta_crit)
        direct_force(point, node, force);
    else {
        for (int i = 0; i < CELLS_PER_NODE; i++) {
            if (node->cell[i] != NULL) //is cell a node?
                add_to_force(point, node->cell[i], force);
            else if (node->leaf[i] != NULL) { //is cell a leaf?
                direct_force(point, node->leaf[i], force);
                //break; //why break?
            }
        }
    }
}

void direct_force(const DATA *point, NODE *node, double *force)
{
    int i;
    double epsilon = 0;
    double ihat;

    double dist_sq = 0;
    for(i = 0; i < 3; i++)
        dist_sq += (point->pos[i] - node->cog[i]) * (point->pos[i] - node->cog[i]);

    for(i = 0; i < 3; i++){
        ihat = (point->pos[i] - node->cog[i]) / sqrt(dist_sq);
        force[i] -= point->mass * node->mass * ihat / (dist_sq + epsilon*epsilon);;
    }
}

int main(int argc, char *argv[])
{
        // Usage: ./main [N epsilon t_step n_steps op_freq integ fname]

    int N = 2;   //number of particles
    double epsilon = 0.0;  //softening parameter
    double t_step = 0.05;  //time step
    int n_steps = 4836;   // number of steps
    int op_freq = 4;    // output frequency
    char *integrator = "LF2";  //ODE integration method
    char *file_name = "init_2body0.5.txt";   //name of initial conditions file

    int i,j,k; //loop variable

    if (argc > 1) {
        if (argc != 8) {
            printf("Usage: ./main [N epsilon t_step n_steps op_freq integ fname]\n");
            return -1;
        }
        //check user input
        for (i=0; i<strlen(argv[1]); i++)
            assert(isdigit(argv[1][i]));
        N = atoi(argv[1]);
        epsilon = atof(argv[2]);
        assert(epsilon >= 0.0);
        t_step = atof(argv[3]);
        assert(t_step >= 1e-8);
        for (i=0; i<strlen(argv[4]); i++)
            assert(isdigit(argv[4][i]));
        n_steps = atoi(argv[4]);
        for (i=0; i<strlen(argv[5]); i++)
            assert(isdigit(argv[5][i]));
        op_freq = atof(argv[5]);
        integrator = argv[6];
        file_name = argv[7];
    }

    double mass[N];
    double position[N][3];
    double velocity[N][3];
    double force[N][3];

    read_data(mass, position, velocity, N, file_name);  //read initial pos and vel from file


	DATA data[N];
	NODE *root = NULL;
    int n = 0;
	while (n <= N) {
		data[n].idx = n;
		data[n].mass = mass[n];
		data[n].pos[0] = position[n][0];
		data[n].pos[1] = position[n][1];
		data[n].pos[2] = position[n][2];
		++n;
    }

    for(i = 0; i < n_steps; i++)
    {
        /* build tree */
        build_tree(n, data, &root);

        /*calculate force on each particle*/

        for(j = 0; j < N; j++)
            for(k = 0; k < 3; k++)
                force[j][k] = 0;   // set all forces to zero

        for(j = 0; j < N; j++) //loop over particles
        {
            double force_j[3] = {0, 0, 0} ;  //force on particle j
            DATA point = data[j];
            add_to_force(point, root, force_j);
            for(k = 0; k < 3; k++)
                force[j][k] = force_j[k];
        }

        /* all done! */
        kill_node(root);

        /*update position*/
        if( strcmp(integrator,"RK4") == 0 )
            integ_RK4(mass, position, velocity, force, N, t_step, epsilon);
        else if ( strcmp(integrator, "LF2") == 0 )
            integ_LF2(mass, position, velocity, force, N, t_step, epsilon);
        else
            return -1;

        /*save current position*/
        if(i % op_freq == 0)
            save_data(mass, position, velocity, N, (int)(i/op_freq));

    }

	return 0;
}

/* bhtree.c */
