#include <stdio.h>
#include <stdlib.h>

const int R = 3;
const int INFTY = 999999999;

int swap_(int *posA, int *posB)
{
    int tmp = *posA;
    *posA = *posB;
    *posB = tmp;
    return 0;
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

void printLevel(int *arr, int idx, int level, int n);
void printLevelOrder(int *arr, int n);

void restoreDown(int *arr, int n, int idx)
{
    int child[R + 1];
    while (1)
    {
        for (int i = 1; i <= R; i++)
        {
            int childIdx = child_(idx, i);
            child[i] = childIdx < n ? childIdx : -1;
        }
        // -1 as ith child means no child

        int min_child = INFTY, min_child_idx;
        for (int i = 1; i <= R; i++)
        {
            int hasIthChild = child[i] != -1;
            int ithChildLessThanMinChild = arr[child[i]] < min_child;
            if (hasIthChild && ithChildLessThanMinChild)
            {
                min_child_idx = child[i];
                min_child = arr[child[i]];
            }
        }
        // is leaf node
        if (min_child == INFTY)
            break;

        int parentValGreaterThanMinChild = (arr[idx] > arr[min_child_idx]);
        if (parentValGreaterThanMinChild)
        {
            // printf("Swapped %d with %d\n", idx, min_child_idx);
            swap_(&arr[idx], &arr[min_child_idx]);
        }

        idx = min_child_idx;
    }
}

void restoreUp(int *arr, int idx)
{
    int parentIdx = parent_(idx);
    while (parentIdx >= 0)
    {
        int parentValueGreaterThanCurr = arr[idx] < arr[parentIdx];
        if (parentValueGreaterThanCurr)
        {
            swap_(&arr[idx], &arr[parentIdx]);
            idx = parentIdx;
            parentIdx = parent_(parentIdx);
        }
        else
            break;
    }
}

void buildHeap(int *arr, int n)
{
    for (int i = (n - 1) / R; i >= 0; i--)
    {
        // printf("Restoring for idx=%d\n", i);
        restoreDown(arr, n, i);
        // printLevelOrder(arr, n);
    }
}

void insert(int *arr, int *n, int val)
{
    arr[*n] = val;
    *n = (*n) + 1;
    restoreUp(arr, *n - 1);
}

int deleteMin(int *arr, int *n)
{
    int min = arr[0];
    arr[0] = arr[*n - 1];
    *n = *n - 1;
    restoreDown(arr, *n, 0);
    return min;
}

int reduceKey(int *arr, int idx, int reduceBy)
{
    arr[idx] -= reduceBy;
    restoreUp(arr, idx);
}

int height(int *arr, int idx, int n)
{
    if (idx >= n)
        return 0;
    else
    {
        int maxHeight = -1;
        for (int i = 1; i <= R; i++)
        {
            int childHeight = height(arr, child_(idx, i), n);
            maxHeight = maxHeight < childHeight ? childHeight : maxHeight;
        }
        return maxHeight + 1;
    }
}

/* Print nodes at a given level */
void printLevel(int *arr, int idx, int level, int n)
{
    if (idx >= n)
    {
        printf("  ");
        return;
    }
    if (level == 1)
        printf("%d ", arr[idx]);
    else if (level > 1)
    {
        for (int i = 1; i <= R; i++)
        {
            int childIdx = child_(idx, i);
            printLevel(arr, childIdx, level - 1, n);
        }
    }
}

void printLevelOrder(int *arr, int n)
{
    int h = height(arr, 0, n) + 1;
    for (int i = 1; i < h; i++)
    {
        printf("[Level %d]: ", i);
        printLevel(arr, 0, i, n);
        printf("\n");
    }
}

void displayArr(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Arguments: [n] [fileName]\n");
        return 1;
    }
    int n = atoi(argv[1]);
    int tmp, arr[n];
    FILE *fp = fopen(argv[2], "r");
    for (int i = 0; i < n; i++)
        tmp = fscanf(fp, "%d ", &arr[i]);
    // int n = 10;
    // int arr[] = {71, 12, 45, 4, 89, 39, 16, 65, 59, 72};

    if (n < 100)
    {
        printf("Initially:\n");
        printLevelOrder(arr, n);
        printf("\n");
    }
    buildHeap(arr, n);
    if (n < 100)
        printLevelOrder(arr, n);
    printf("\n");

    printf("Extracting 1/5th of Minima:\n");
    for (int i = 0; i < n / 5; i++)
    {
        int min = deleteMin(arr, &n);
        printf("%d ", min);
    }
    printf("\n\n");

    if (n < 100)
        printLevelOrder(arr, n);

    printf("Trying reduce key:\n");
    reduceKey(arr, n - 1, 81);

    if (n < 100)
        printLevelOrder(arr, n);
    return 0;
}