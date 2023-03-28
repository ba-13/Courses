#include "shared.h"

void mergeConsecutiveArr(int *array, int left, int mid, int right);
int min(int x, int y) { return x < y ? x : y; }

int mergeSort(int *array, int n)
{
    for (int curr_size = 1; curr_size < n; curr_size *= 2)
    {
        for (int left_ = 0; left_ < n - 1; left_ += 2 * curr_size)
        {
            int mid = min(left_ + curr_size - 1, n - 1);
            int right_ = min(left_ + 2 * curr_size - 1, n - 1);

            mergeConsecutiveArr(array, left_, mid, right_);
        }
    }
    return 0;
}

void mergeConsecutiveArr(int *array, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int LeftArr[n1], RightArr[n2];
    for (int i = 0; i < n1; i++)
        LeftArr[i] = array[left + i]; // (left) to (mid)
    for (int i = 0; i < n2; i++)
        RightArr[i] = array[mid + i + 1]; // (mid + 1) to (right)

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (LeftArr[i] < RightArr[j])
        {
            array[k] = LeftArr[i];
            i++;
        }
        else
        {
            array[k] = RightArr[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        array[k] = LeftArr[i];
        k++, i++;
    }
    while (j < n2)
    {
        array[k] = RightArr[j];
        k++, j++;
    }
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

    gettimeofday(&t1, NULL);

    mergeSort(numbers, n);

    gettimeofday(&t2, NULL);

    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;    // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms

    printf("%f ms.\n", elapsedTime);

    filePointer = fopen("_sorted_numbers.txt", "w");
    for (int i = 0; i < n; i++)
        fprintf(filePointer, "%d ", numbers[i]);
    fclose(filePointer);

    return 0;
}