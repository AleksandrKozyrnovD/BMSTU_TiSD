#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdio.h>
#include <stdlib.h>

#include "adjmat.h"

typedef struct
{
    int fvertex;    // вершина 1
    int svertex;    // вершина 2
} edge_t;

typedef struct
{
    int size;       // размер цепочки
    edge_t *edges;  // указатель на рёбра
} chain_t;

chain_t *chcreate(adjmat_t matrix);

void chfree(chain_t *chain);

edge_t **alrows(const int n, const int m);

void frrows(edge_t **data, const int n);

void gvexport(adjmat_t matrix, adjmat_t result);

#endif
