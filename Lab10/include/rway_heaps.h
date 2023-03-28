#include <stdlib.h>
#include <stdio.h>

static const int INFTY = 999999999;
static const int R = 3;

typedef struct MinHeapNode 
{
    int v;
    int key;
} MinHeapNode;

typedef struct MinHeap
{
    int size;
    int capacity;
    int *position; // to store position of each vertex in minheap
    MinHeapNode **heap;
} MinHeap;

void print_array(int *arr, int n);

void swap_MinHeapNode(MinHeapNode **a, MinHeapNode **b);

int parent_(int idx);

int child_(int idx, int childNumber);

MinHeapNode *create_MinHeapNode(int v, int key);

MinHeap *create_MinHeap(int capacity);

void restoreDown(MinHeap *mh, int idx);

void restoreUp(MinHeap *mh, int idx);

int isInMinHeap(MinHeap *mh, int v);

MinHeapNode *deleteMin(MinHeap *mh);

void reduceKey(MinHeap *mh, int v, int key);
