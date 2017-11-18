#ifndef definitions_h
#define definitions_h


#define DIM 3  /*number of dimensions*/

/* particle definitions */

#define MAX_NUM_PART 5000

typedef struct {
  int idx;
  double mass;
  double pos[3];
  double vel[3];
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

#endif
