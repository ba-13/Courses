#include "shared.h"

int mergingCount = 0;

void merge(int *arr, int below, int mid, int above)
{
    mergingCount++;
    int n1 = mid - below + 1;
    int n2 = above - mid;
    int LeftArr[n1], RightArr[n2];
    for (int i = 0; i < n1; i++)
        LeftArr[i] = arr[below + i];
    for (int i = 0; i < n2; i++)
        RightArr[i] = arr[mid + i + 1];

    int i = 0, j = 0, k = below;
    while (i < n1 && j < n2)
    {
        if (LeftArr[i] < RightArr[j])
        {
            arr[k] = LeftArr[i];
            i++;
        }
        else
        {
            arr[k] = RightArr[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = LeftArr[i];
        i++, k++;
    }
    while (j < n2)
    {
        arr[k] = RightArr[j];
        j++, k++;
    }
}

void mergeSort(int *arr, int below, int above)
{
    int mid = (below + above) / 2;
    if (below >= above)
    {
        return;
    }
    if (below == above - 1)
    {
        if (arr[below] > arr[above])
            swap(&(arr[below]), &(arr[above]));
        return;
    }
    mergeSort(arr, below, mid);
    mergeSort(arr, mid + 1, above);
    merge(arr, below, mid, above);
}

int main(int argc, char **argv)
{
    array *arr = handleArguments(argc, argv);
    if (arr->n == -1)
        return 1;

    srand(time(NULL));
    gettimeofday(&t1, NULL);
    mergeSort(arr->arr, 0, arr->n - 1);
    gettimeofday(&t2, NULL);

    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;    // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms

    printf("%f ms.\n", elapsedTime);
    printf("Number of merges performed: %d\n", mergingCount);

    FILE *filePointer = fopen("_sorted_numbers.txt", "w");
    for (int i = 0; i < arr->n; i++)
        fprintf(filePointer, "%d ", arr->arr[i]);
    fclose(filePointer);

    return 0;
}
