#include "shared.h"

void insertionSort(int *array, int n)
{
    for (int k = 0; k < n; k++)
    {
        // to print progress
        if (k % (int)(n / 10) == 0)
            fprintf(stderr, "█ %d per\n", (k / (n / 10)) * 10);

        // sorted till k
        // check position of kth number
        for (int i = k - 1; i >= 0; i--)
        {
            if (array[i + 1] < array[i])
            {
                int tmp = array[i + 1];
                array[i + 1] = array[i];
                array[i] = tmp;
            }
            else
                break;
        }
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

    // displayArr(numbers, n);
    insertionSort(numbers, n);
    // displayArr(numbers, n);

    filePointer = fopen("_sorted_numbers.txt", "w");
    for (int i = 0; i < n; i++)
        fprintf(filePointer, "%d ", numbers[i]);
    fclose(filePointer);

    return 0;
}