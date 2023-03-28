#include <stdio.h>
#include <stdlib.h>

typedef struct edge
{
    int n1, n2, w;
} edge;

typedef struct node
{
    int rank, parent;
} node;

void swap(edge *a, edge *b);

void print_edges(edge *edges, int n);

int partition(edge *edges, int p, int r);

void quick_sort(edge *edges, int b, int a);

int find_parent(node *nodes, int node_);

void union_set(node *nodes, int pIdx, int qIdx);

void kruskal(edge *edges, int n, int m);