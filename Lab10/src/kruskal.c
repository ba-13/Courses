#include "kruskal.h"

void swap(edge *a, edge *b)
{
    edge tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(edge *edges, int p, int r)
{
    edge pivot = edges[r];
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        if (edges[j].w <= pivot.w)
        {
            i += 1;
            swap(&edges[i], &edges[j]);
        }
    }
    swap(&edges[i + 1], &edges[r]);
    return i + 1;
}

void quick_sort(edge *edges, int b, int a)
{
    if (b >= a)
        return;
    int pivotIdx = partition(edges, b, a);
    quick_sort(edges, b, pivotIdx - 1);
    quick_sort(edges, pivotIdx + 1, a);
}

void print_edges(edge *edges, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d --(%d)-- %d\n", edges[i].n1, edges[i].w, edges[i].n2);
    printf("\n");
}

int find_parent(node *nodes, int nodeIdx)
{
    if (nodes[nodeIdx].parent == nodeIdx)
    {
        return nodeIdx;
    }
    nodes[nodeIdx].parent = find_parent(nodes, nodes[nodeIdx].parent);
    return nodes[nodeIdx].parent;
}

void union_set(node *nodes, int pIdx, int qIdx)
{
    pIdx = find_parent(nodes, pIdx);
    qIdx = find_parent(nodes, qIdx);
    if (nodes[pIdx].rank < nodes[qIdx].rank)
    {
        nodes[pIdx].parent = qIdx;
    }
    else if (nodes[pIdx].rank > nodes[qIdx].rank)
    {
        nodes[qIdx].parent = pIdx;
    }
    else
    {
        nodes[qIdx].parent = pIdx;
        nodes[pIdx].rank++;
    }
}

void kruskal(edge *edges, int n, int m)
{
    quick_sort(edges, 0, n - 1);
    printf("Weight sorted edges:\n");
    print_edges(edges, n);
    // now create a list of graph nodes
    node nodes[m];
    for (int i = 0; i < n; i++)
    {
        nodes[i].parent = i;
        nodes[i].rank = 0;
    }

    for (int i = 0; i < n; i++)
    {
        // for each edge
        int v1 = find_parent(nodes, edges[i].n1);
        int v2 = find_parent(nodes, edges[i].n2);
        int wt = edges[i].w;
        if (v1 != v2)
        {
            union_set(nodes, v1, v2);
            printf("%d --(%d)-- %d\n", edges[i].n1, wt, edges[i].n2);
        }
    }
}

/**
 * Would be receiving an edge list
 */
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

    kruskal(edges, n, m);

    return 0;
}