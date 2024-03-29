#include "shared.h"

int partition(int *arr, int p, int r)
{
    int pivotIdx = r;
    int pivot = arr[pivotIdx];
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(&(arr[i]), &(arr[j]));
        }
    }
    swap(&(arr[i + 1]), &(arr[r]));
    return i + 1;
}

void quickSort(int *arr, int b, int a)
{
    if (b >= a)
        return;
    int pivot = partition(arr, b, a);
    quickSort(arr, b, pivot - 1);
    quickSort(arr, pivot + 1, a);
}

int orderStaticSelect(int *arr, int p, int r, int k)
{
    if (p == r)
    {
        if (k == 1)
            ;
        else
            printf("k not equal to 1!\n");
        return p;
    }
    int pivotIdx = partition(arr, p, r);
    // number of elements smaller than pivot
    int k_ = pivotIdx - p + 1;
    if (k == k_)
        // pivot is the kth element
        return pivotIdx;
    else if (k < k_)
        // k is smaller than pivotLocation
        return orderStaticSelect(arr, p, pivotIdx - 1, k);
    else
        // k is larger than pivotLocation
        return orderStaticSelect(arr, pivotIdx + 1, r, k - k_);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Arguments needed: [filename] [n]\n");
        return 1;
    }
    int n = atoi(argv[2]);
    FILE *fp = fopen(argv[1], "r");
    int numbers[n], tmp;
    for (int i = 0; i < n; i++)
        tmp = fscanf(fp, "%d ", &numbers[i]);
    fclose(fp);

    gettimeofday(&t1, NULL);
    int medianLocation = n / 2;
    int medianIdx = orderStaticSelect(numbers, 0, n - 1, medianLocation);
    gettimeofday(&t2, NULL);

    printf("Time taken by select(): %fms\n", (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_usec - t1.tv_usec) / 1000.0);

    printf("%f order static: %d\n", (medianLocation / (float)n), numbers[medianIdx]);

    fp = fopen("_selected_number.txt", "w");
    for (int i = 0; i < n; i++)
        fprintf(fp, "%d ", numbers[i]);
    fclose(fp);

    fp = fopen(argv[1], "r");
    for (int i = 0; i < n; i++)
        tmp = fscanf(fp, "%d ", &numbers[i]);
    fclose(fp);

    gettimeofday(&t1, NULL);
    quickSort(numbers, 0, n - 1);
    gettimeofday(&t2, NULL);

    printf("Time taken by quickSort(): %fms\n", (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_usec - t1.tv_usec) / 1000.0);

    medianIdx = n / 2 - 1;
    printf("%f order static: %d\n", (medianLocation / (float)n), numbers[medianIdx]);

    fp = fopen("_sorted_numbers.txt", "w");
    for (int i = 0; i < n; i++)
        fprintf(fp, "%d ", numbers[i]);
    fclose(fp);

    return 0;
}
