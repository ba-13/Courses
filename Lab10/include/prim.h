#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct edge
{
    int n1, n2, w;
} edge;

void edges_to_adjacency_matrix(edge *edges, int n, int **mat);

void printArr(int *arr, int n);