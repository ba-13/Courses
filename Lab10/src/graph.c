#include "graph.h"

AdjListNode *create_AdjListNode(int dest, int weight)
{
    AdjListNode *newNode = (AdjListNode *)malloc(sizeof(AdjListNode));
    newNode->destination = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

Graph *create_Graph(int V)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;

    graph->array = (AdjList *)malloc(V * sizeof(AdjList));

    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

void insert_edge(Graph *graph, int src, int dest, int weight)
{
    AdjListNode *newNode = create_AdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = create_AdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}
