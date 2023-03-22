#include "shared.h"

int partition(int *arr, int p, int r, int pivotIdx)
{
    int pivot = arr[pivotIdx];
    swap(&arr[pivotIdx], &arr[r]); // move pivot to last place
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[r]);
    return i + 1;
}

void quickSort(int *arr, int b, int a)
{
    if (b >= a)
        return;
    int pivot = partition(arr, b, a, a);
    quickSort(arr, b, arr - 1);
    quickSort(arr, pivot + 1, a);
}

// for smaller arrays
int find_median(int *arr, int b, int a)
{
    quickSort(arr, b, a);
    return (b + a) / 2;
}

int select(int *arr, int b, int a, int k)
{
    int n = a - b + 1;
    static const int group_size = 5;
    int C[n / group_size];
    int count = 0;
    for (int idx = 0; idx < n; idx += group_size)
    {
        C[count] = find_median(arr, idx, idx + group_size);
        count++;
    }
    for (; count < n / group_size; count++)
        C[count] = -99999999;
    int pivotIdx = select(arr, b, a, n / 2);   // obtain the good pivot index
    pivotIdx = partition(arr, b, a, pivotIdx); // put it in actual pivot position
    if (pivotIdx - a + 1 >= k)
        return select(arr, b, pivotIdx - 1, k);
    else if (pivotIdx - a + 1 == k - 1)
        return pivotIdx;
    else
        return select(arr, pivotIdx + 1, a, k - (pivotIdx - a + 1) - 1);
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
