#include "shared.h"

int swap(int *posA, int *posB)
{
    int tmp = *posA;
    *posA = *posB;
    *posB = tmp;
    return 0;
}

/**
 * @brief Should be called when minimum element is removed (i.e. idx = 1)
 *
 * @param heap
 * @param n
 * @return int
 */
int heapify(int *heap, int n) // we have heap from 1 to n(inclusive)
{
    int idx = 1;
    while (idx <= n)
    {
        if (2 * idx + 1 <= n) // both children
        {
            if (heap[idx] > heap[2 * idx] && heap[2 * idx + 1] >= heap[2 * idx])
            {
                // (2 * idx) is smallest
                swap(&heap[idx], &heap[2 * idx]);
                idx = 2 * idx;
            }
            else if (heap[idx] > heap[2 * idx + 1] && heap[2 * idx] >= heap[2 * idx + 1])
            {
                // (2 * idx + 1) is smallest
                swap(&heap[idx], &heap[2 * idx + 1]);
                idx = 2 * idx + 1;
            }
            else
                break;
        }
        else if (2 * idx <= n) // one left hand child
        {
            if (heap[idx] > heap[2 * idx])
            {
                swap(&heap[idx], &heap[2 * idx]);
                idx = 2 * idx;
            }
            else
                break;
        }
        else // no children
            break;
    }
}

int insertHeap(int val, int *heap, int *heapSize)
{
    (*heapSize)++;
    int curr = *heapSize;
    while (curr / 2 >= 1 && heap[curr / 2] > val)
    {
        heap[curr] = heap[curr / 2]; // put parent to child position
        curr /= 2;
    }
    heap[curr] = val;
    return 0;
}

void heapify_(int arr[], int N, int i)
{
    int largest = i;   // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < N && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < N && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i)
    {
        swap(&arr[i], &arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify_(arr, N, largest);
    }
}

// Function to build a Max-Heap from the given array
void buildHeap(int arr[], int N)
{
    // Index of last non-leaf node
    int startIdx = (N / 2) - 1;

    // Perform reverse level order traversal
    // from last non-leaf node and heapify
    // each node
    for (int i = startIdx; i >= 0; i--)
    {
        heapify_(arr, N, i);
    }
}

void initializeHeap(int *heap)
{
    heap[0] = -999999999; // placeholder at 0th index which is not used
}

int heapSort(int *arr, int n)
{
    int heap[n + 1];
    int heapSize = 0;
    initializeHeap(heap);

    for (int i = 0; i < n; i++)
        insertHeap(arr[i], heap, &heapSize);

    for (int i = 0; i < n; i++)
    {
        arr[i] = heap[1];
        swap(&heap[1], &heap[heapSize]); // move minima out of heap
        heapSize--;
        heapify(heap, heapSize);
    }
    // displayArr(arr, n);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Arguments needed: [fileName] [n]\n");
        fprintf(stderr, "fileName - file path containing numbers to be sorted\n");
        fprintf(stderr, "n - #numbers in file\n");
        return 1;
    }

    FILE *filePointer;
    filePointer = fopen(argv[1], "r");
    int n = atoi(argv[2]);
    int numbers[n];
    for (int i = 0; i < n; i++)
    {
        int tmp = fscanf(filePointer, "%d ", &numbers[i]);
    }
    fclose(filePointer);

    // int n_ = 6;
    // int arr[] = {34, 132, 100, -131, 10, 10};
    // heapSort(arr, n_);

    heapSort(numbers, n);

    filePointer = fopen("_sorted_numbers.txt", "w");
    for (int i = 0; i < n; i++)
        fprintf(filePointer, "%d ", numbers[i]);
    fclose(filePointer);
    return 0;
}