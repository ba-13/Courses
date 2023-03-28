#include <stdlib.h>

typedef struct AdjListNode
{
    int destination;
    int weight;
    struct AdjListNode *next;
} AdjListNode;

typedef struct AdjList
{
    struct AdjListNode *head;
} AdjList;

typedef struct Graph
{
    int V;
    struct AdjList *array;
} Graph;

AdjListNode *create_AdjListNode(int dest, int weight);

Graph *create_Graph(int V);

void insert_edge(Graph *graph, int src, int dest, int weight);
