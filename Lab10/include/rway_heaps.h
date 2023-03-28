#include <stdlib.h>

static const int INFTY = 999999999;
static const int R = 3;

typedef struct minheap_node
{
    int v;
    int key;
} minheap_node;

typedef struct minheap
{
    int size;
    int capacity;
    int *position; // to store position of each vertex in minheap
    minheap_node **heap;
} minheap;

void swap_(minheap_node **posA, minheap_node **posB);

int parent_(int idx);

int child_(int idx, int childNumber);

minheap_node *create_minheap_node(int v, int key);

minheap *create_minheap(int capacity);

void restoreDown(minheap *mh, int idx);

void restoreUp(minheap *mh, int idx);

int isInMinHeap(minheap *mh, int v);

minheap_node *deleteMin(minheap *mh);

void reduceKey(minheap *mh, int v, int key);