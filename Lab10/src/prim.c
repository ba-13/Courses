#include "prim.h"
#include "rway_heaps.h"
#include "graph.h"

void edges_to_adjacency_matrix(edge *edges, int n, int **mat)
{
    for (int i = 0; i < n; i++)
    {
        mat[edges[i].n1][edges[i].n2] = edges[i].w;
        mat[edges[i].n2][edges[i].n1] = edges[i].w;
    }
}

void printArr(int *arr, int n)
{
    for (int i = 1; i < n; ++i)
        printf("%d - %d\n", arr[i], i);
}

void prim(graph *G)
{
    int V = G->V;
    int parent[V];
    int key[V];

    struct minheap *minHeap = create_minheap(V);

    // Initialize min heap with all vertices. Key value of
    // all vertices (except 0th vertex) is initially
    // infinite
    for (int v = 1; v < V; ++v)
    {
        parent[v] = -1;
        key[v] = INFTY;
        minHeap->heap[v] = create_minheap_node(v, key[v]);
        minHeap->position[v] = v;
    }

    // Make key value of 0th vertex as 0 so that it
    // is extracted first
    key[0] = 0;
    minHeap->heap[0] = create_minheap_node(0, key[0]);
    minHeap->position[0] = 0;

    // Initially size of min heap is equal to V
    minHeap->size = V;

    // In the following loop, min heap contains all nodes
    // not yet added to MST.
    while (minHeap->size != 0)
    {
        // Extract the vertex with minimum key value
        minheap_node *minHeapNode = deleteMin(minHeap);
        int u = minHeapNode
                    ->v; // Store the extracted vertex number

        // Traverse through all adjacent vertices of u (the
        // extracted vertex) and update their key values
        struct node *pCrawl = G->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->destination;

            // If v is not yet included in MST and weight of
            // u-v is less than key value of v, then update
            // key value and parent of v
            if (isInMinHeap(minHeap, v) && pCrawl->weight < key[v])
            {
                key[v] = pCrawl->weight;
                parent[v] = u;
                reduceKey(minHeap, v, key[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    printArr(parent, V);
}

int main(int argc, char **argv)
{
    int n = 10;
    int m = 7;
    edge edges[] = {{0, 5, 3},
                    {0, 4, 1},
                    {1, 5, 4},
                    {5, 3, 1},
                    {1, 2, 5},
                    {2, 4, 9},
                    {2, 3, 7},
                    {2, 6, 2},
                    {5, 6, 8},
                    {4, 6, 3}};

    graph *G = create_graph(m);
    for (int i = 0; i < n; i++)
    {
        add_edge(G, edges[i].n1, edges[i].n2, edges[i].w);
    }

    prim(G);

    return 0;
}