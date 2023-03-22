#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

struct timeval t1, t2;
double elapsedTime;

void displayArr(int *array, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("\n");
}