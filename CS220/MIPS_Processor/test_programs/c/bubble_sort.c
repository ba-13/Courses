#include <stdio.h>

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void bubble_sort(int *arr, int n)
{
    int swapped = 0;
    do
    {
        swapped = 0;
        for (int i = 1; i < n; i++)
        {
            if (arr[i - 1] > arr[i])
            {
                swap(&arr[i - 1], &arr[i]);
                swapped = 1;
            }
        }
    } while (swapped == 1);
}

void display_arr(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main()
{
    int n = 15;
    int arr[] = {86, 60, 136, 25, 97, 10, 56, 98, 48, 49, 113, 126, 48, 0, 72};
    display_arr(arr, n);
    bubble_sort(arr, n);
    display_arr(arr, n);

    return 0;
}