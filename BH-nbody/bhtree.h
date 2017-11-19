/*
 ** bhtree.h
 ** ========
 ** Demo of Barnes & Hut tree code.
 **
 ** Derek C. Richardson 10/25/17
 */

#ifndef bhtree_h
#define bhtree_h

#include "definitions.h"

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

#endif
/* bhtree.h */
