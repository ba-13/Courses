#include <stdlib.h>

typedef struct node {
    int destination, weight;
    struct node *next;
} node;

typedef struct list {
    node *head;
} list;

typedef struct graph {
    int V;
    list *array;
} graph;

node *create_adjacency_node(int destination, int weight);

graph *create_graph(int V);

void add_edge(graph *G, int src, int dst, int wt);