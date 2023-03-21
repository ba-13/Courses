#include <stdio.h>
#include <stdlib.h>

void displayArr(int *array, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("\n");
}