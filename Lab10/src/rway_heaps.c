#include "rway_heaps.h"

void swap_(minheap_node **posA, minheap_node **posB)
{
    minheap_node *tmp = *posA;
    *posA = *posB;
    *posB = tmp;
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

minheap_node *create_minheap_node(int v, int key)
{
    minheap_node *tmp = (minheap_node *)malloc(
        sizeof(minheap_node));
    tmp->v = v;
    tmp->key = key;
    return tmp;
}

minheap *create_minheap(int capacity)
{
    minheap *minHeap = (minheap *)malloc(sizeof(minheap));
    minHeap->position = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->heap = (minheap_node **)malloc(
        capacity * sizeof(minheap_node *));
    return minHeap;
}

void restoreDown(minheap *mh, int idx)
{
    int child[R + 1]; // stores the children of idx
    while (1)
    {
        for (int i = 1; i <= R; i++)
        {
            int childIdx = child_(idx, i);
            child[i] = childIdx < mh->size ? childIdx : -1;
        }
        // -1 as ith child means no child

        int min_child = INFTY, min_child_idx;
        for (int i = 1; i <= R; i++)
        {
            int hasIthChild = (child[i] != -1);
            int ithChildLessThanMinChild = (mh->heap[child[i]]->key < min_child);
            if (hasIthChild && ithChildLessThanMinChild)
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
            swap_(&mh->heap[idx], &mh->heap[min_child_idx]);
        }

        idx = min_child_idx;
    }
}

void restoreUp(minheap *mh, int idx)
{
    int parentIdx = parent_(idx);
    while (parentIdx >= 0)
    {
        int parentValueGreaterThanCurr = (mh->heap[idx]->key < mh->heap[parentIdx]->key);
        if (parentValueGreaterThanCurr)
        {
            swap_(&mh->heap[idx], &mh->heap[parentIdx]);
            idx = parentIdx;
            parentIdx = parent_(parentIdx);
        }
        else
            break;
    }
}

int isInMinHeap(minheap *mh, int v)
{
    if (mh->position[v] < mh->size)
        return 1;
    return 0;
}

minheap_node *deleteMin(minheap *mh)
{
    if (mh->size == 0)
        return NULL;
    minheap_node *min = mh->heap[0];
    minheap_node *lastNode = mh->heap[mh->size - 1];
    mh->heap[0] = lastNode;

    mh->position[min->v] = mh->size - 1;
    mh->position[lastNode->v] = 0;

    mh->size -= 1;

    restoreDown(mh, 0);

    return min;
}

void reduceKey(minheap *mh, int v, int key)
{
    int i = mh->position[v];
    mh->heap[i]->key = key;
    restoreUp(mh, i);
}