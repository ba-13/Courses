#include <time.h>
#include "shared.h"

void random_number(int n, double *result, int min_num, int max_num);

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "Arguments needed: [n] [lower] [higher]\n");
        fprintf(stderr, "n - #numbers\n");
        fprintf(stderr, "lower - lower number in range\n");
        fprintf(stderr, "higher - higher number in range\n");
        return 1;
    }

    int n = atoi(argv[1]), lower = atoi(argv[2]), higher = atoi(argv[3]);

    double random_numbers[n];
    random_number(n, random_numbers, lower, higher);

    FILE *filePointer;
    filePointer = fopen("_random_numbers.txt", "w");

    for (int i = 0; i < n; i++)
        fprintf(filePointer, "%0.1lf ", random_numbers[i]);

    fclose(filePointer);
    return 0;
}

void random_number(int n, double *result, int min_num, int max_num)
{
    int low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; // include max_num in output
    }
    else
    {
        low_num = max_num + 1; // include max_num in output
        hi_num = min_num;
    }

    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        double tmp = rand() % (hi_num - low_num);
        result[i] = (float)rand() / (float)(RAND_MAX / (max_num - min_num)) + min_num;
        ;
    }
}
