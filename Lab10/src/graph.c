#include "graph.h"

node *create_adjacency_node(int destination, int weight)
{
    node *tmp = (node *)malloc(sizeof(node));
    tmp->destination = destination;
    tmp->weight = weight;
    tmp->next = NULL;
    return tmp;
}

graph *create_graph(int V)
{
    graph *tmp = (graph *)malloc(sizeof(graph));
    tmp->V = V;
    tmp->array = (list *)malloc(V * sizeof(list));

    for (int i = 0; i < V; i++)
        tmp->array[i].head = NULL;

    return tmp;
}

void add_edge(graph *G, int src, int dst, int wt)
{
    node *n1 = create_adjacency_node(src, wt);
    node *n2 = create_adjacency_node(dst, wt);
    n1->next = G->array[dst].head;
    G->array[dst].head = n1;
    n2->next = G->array[src].head;
    G->array[src].head = n2;
}