#include "shared.h"

int badPivotCount = 0;
int goodPivotCount = 0;

int partition(int *arr, int p, int r)
{
    int pivot = arr[r];
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        if (arr[j] <= pivot)
        {
            i += 1;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&(arr[i + 1]), &(arr[r]));

    if (i + 1 - p < (r - p) / 5)
    {
        badPivotCount++;
        // partition(arr, p, r);
    }
    else
    {
        goodPivotCount++;
    }
    return i + 1;
}

int partition_random(int arr[], int p, int r)
{
    int idx = p + rand() % (r - p + 1);
    int pivot = arr[idx];
    swap(&(arr[idx]), &(arr[r]));
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        if (arr[j] <= pivot)
        {
            i += 1;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&(arr[i + 1]), &(arr[r]));
    if (i + 1 - p < (r - p) / 5)
    {
        badPivotCount++;
        // partition(arr, p, r);
    }
    else
    {
        goodPivotCount++;
        return i + 1;
    }
}

void quickSort(int arr[], int below, int above)
{
    if (below >= above)
    {
        return;
    }
    int pivotIdx = partition_random(arr, below, above);
    quickSort(arr, below, pivotIdx - 1);
    quickSort(arr, pivotIdx + 1, above);
}

/*
int main()
{
    int n = 10;
    int arr[] = {4, 6, 7, 8, 9, 8, 6, 0, 1, 6};
    displayArr_(arr, n);
    quickSort(arr, 0, n - 1);
    displayArr_(arr, n);
    return 0;
}
*/

int main(int argc, char **argv)
{
    array *arr = handleArguments(argc, argv);
    if (arr->n == -1)
        return 1;

    srand(time(NULL));
    gettimeofday(&t1, NULL);
    quickSort(arr->arr, 0, arr->n - 1);
    gettimeofday(&t2, NULL);

    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;    // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms

    printf("%f ms.\n", elapsedTime);
    printf("Bad Pivots chosen: %d\n", badPivotCount);
    printf("Good Pivots chosen: %d\n", goodPivotCount);
    printf("Chance of getting a bad pivot: %f\n", ((float)badPivotCount / (goodPivotCount + badPivotCount) * 1.0f));

    FILE *filePointer = fopen("_sorted_numbers.txt", "w");
    for (int i = 0; i < arr->n; i++)
        fprintf(filePointer, "%d ", arr->arr[i]);
    fclose(filePointer);

    return 0;
}
