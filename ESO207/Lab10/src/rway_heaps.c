#include "rway_heaps.h"

void swap_MinHeapNode(MinHeapNode **a,
                     MinHeapNode **b)
{
    MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

int parent_(int idx)
{
    return (idx - 1) / R;
}

int child_(int idx, int childNumber)
{
    // childNumber varies between 1 to R
    if (childNumber < 1 || childNumber > R)
    {
        exit(1);
    }
    return R * idx + childNumber;
}

MinHeapNode *create_MinHeapNode(int v, int key)
{
    MinHeapNode *minHeapNode = (MinHeapNode *)malloc(
        sizeof(MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->key = key;
    return minHeapNode;
}

MinHeap *create_MinHeap(int capacity)
{
    MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));
    minHeap->position = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->heap = (MinHeapNode **)malloc(
        capacity * sizeof(MinHeapNode *));
    return minHeap;
}

void print_array(int *arr, int n) {
  for (int i=0; i<n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void restoreDown(MinHeap *mh, int idx)
{
    int child[R + 1]; // stores the children of idx
    while (1)
    {
        for (int i = 1; i <= R; i++)
        {
            int childIdx = child_(idx, i);
            child[i] = (childIdx < mh->size) ? childIdx : -1;
        }
        // print_array(child, R+1);
        // -1 as ith child means no child

        int min_child = INFTY, min_child_idx;
        for (int i = 1; i <= R; i++)
        {
            int hasIthChild = (child[i] != -1);
            if (!hasIthChild) continue;
            int ithChildLessThanMinChild = (mh->heap[child[i]]->key < min_child);
            if (ithChildLessThanMinChild)
            {
                min_child_idx = child[i];
                min_child = mh->heap[child[i]]->key;
            }
        }
        // is leaf node
        if (min_child == INFTY)
            break;

        int parentValGreaterThanMinChild = (mh->heap[idx]->key > mh->heap[min_child_idx]->key);
        if (parentValGreaterThanMinChild)
        {
            swap_MinHeapNode(&mh->heap[idx], &mh->heap[min_child_idx]);
        }

        idx = min_child_idx;
    }
}

void restoreUp(MinHeap *mh, int idx)
{
    int parentIdx = parent_(idx);
    while (parentIdx >= 0)
    {
        int parentValueGreaterThanCurr = (mh->heap[idx]->key < mh->heap[parentIdx]->key);
        if (parentValueGreaterThanCurr)
        {
            swap_MinHeapNode(&mh->heap[idx], &mh->heap[parentIdx]);
            idx = parentIdx;
            parentIdx = parent_(parentIdx);
        }
        else
            break;
    }
}

// vertex v is in minHeap or not
int isInMinHeap(MinHeap *mh, int v)
{
    if (mh->position[v] < mh->size)
        return 1;
    return 0;
}

MinHeapNode *deleteMin(MinHeap *mh)
{
    if (mh->size == 0)
        return NULL;
    MinHeapNode *min = mh->heap[0];
    MinHeapNode *lastNode = mh->heap[mh->size - 1];
    mh->heap[0] = lastNode;

    mh->position[min->v] = mh->size - 1;
    mh->position[lastNode->v] = 0;

    mh->size -= 1;

    restoreDown(mh, 0);

    return min;
}

void reduceKey(MinHeap *mh, int v, int key)
{
    int i = mh->position[v];
    mh->heap[i]->key = key;
    restoreUp(mh, i);
}
