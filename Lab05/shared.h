#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

struct timeval t1, t2;
double elapsedTime;

typedef struct array
{
    int *arr;
    int n;
} array;

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void displayArr(array *arr)
{
    for (int i = 0; i < arr->n; i++)
        printf("%d ", arr->arr[i]);
    printf("\n");
}

void displayArr_(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

array *handleArguments(int argc, char **argv)
{
    array *arr = (array *)malloc(sizeof(array));
    if (argc < 3)
    {
        fprintf(stderr, "Arguments needed: [fileName] [n]\n");
        fprintf(stderr, "fileName - file path containing numbers to be sorted\n");
        fprintf(stderr, "n - #numbers in file\n");
        arr->arr = NULL;
        arr->n = -1;
        return arr;
    }

    FILE *filePointer;
    filePointer = fopen(argv[1], "r");
    int n = atoi(argv[2]);
    int *numbers = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        int tmp = fscanf(filePointer, "%d ", &numbers[i]);
    }
    fclose(filePointer);

    arr->arr = numbers;
    arr->n = n;
    return arr;
}