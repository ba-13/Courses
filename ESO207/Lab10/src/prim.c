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

void print_mst(int *arr, int *keys, int n)
{
    for (int i = 1; i < n; ++i)
        printf("%d --(%d)-- %d\n", arr[i], keys[i], i);
}

void prim(Graph *G)
{
    int V = G->V;
    int parent[V]; // would store the tree of MST once completed
    int key[V]; // stores the current distance from MST set

    // initialise minHeap
    MinHeap *minHeap = create_MinHeap(V);

    for (int v = 1; v < V; ++v)
    {
        parent[v] = -1;
        key[v] = INFTY;
        minHeap->heap[v] = create_MinHeapNode(v, key[v]);
        minHeap->position[v] = v; // stores the position of vertex v in minHeap
    }

    key[0] = 0;
    // have to initialise the root as the minimum distance vertex
    minHeap->heap[0] = create_MinHeapNode(0, key[0]);
    minHeap->position[0] = 0;

    minHeap->size = V;

    while (minHeap->size != 0)
    {
        // deleteMin done on basis of key value of nodes
        MinHeapNode *minHeapNode = deleteMin(minHeap);
        // vertex number
        int u = minHeapNode->v;

        AdjListNode *uNeighbors = G->array[u].head;
        while (uNeighbors != NULL) // iterating through u's neighbors
        {
            int v = uNeighbors->destination; // it's neighbor

            // check if v has not been visited, and it's key greater than the
            // edge to v
            if (isInMinHeap(minHeap, v) && uNeighbors->weight < key[v])
            {
                key[v] = uNeighbors->weight;
                parent[v] = u;
                reduceKey(minHeap, v, key[v]);
            }
            uNeighbors = uNeighbors->next;
        }
    }

    print_mst(parent, key, V);
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

    Graph *G = create_Graph(m);
    for (int i = 0; i < n; i++)
    {
        insert_edge(G, edges[i].n1, edges[i].n2, edges[i].w);
    }

    prim(G);

    return 0;
}
