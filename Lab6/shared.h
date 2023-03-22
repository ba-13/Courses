#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

struct timeval t1, t2;
double elapsedTime;
int verbose = 1;

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void displayArr(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void displaymat(int *mat, int n, char *matrixName)
{
    printf("%s:\n", matrixName);
    for (int i = 0; i < n; i++)
    { // iterate over rows
        for (int j = 0; j < n; j++)
        {
            printf("\t%d", mat[n * i + j]);
        }
        printf("\n");
    }
    for (int i = 0; i < n; i++)
        printf("\t");
    printf("\t(%dx%d)\n", n, n);
}
