#include "shared.h"

void multiply(int *mat1, int *mat2, int *mat3, int K)
{
    for (int i = 0; i < K; i++)
        for (int j = 0; j < K; j++)
            mat3[K * i + j] = 0; // initialize

    for (int i = 0; i < K; i++)
        for (int j = 0; j < K; j++)
            for (int h = 0; h < K; h++)
            {
                mat3[K * i + j] += mat1[K * i + h] * mat2[K * h + j];
            }
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Arguments: [filename] [k] [verbose]\n");
        return 1;
    }
    verbose = atoi(argv[3]);

    FILE *fp = fopen(argv[1], "r");
    int k = atoi(argv[2]), tmp;
    int mat1[k * k], mat2[k * k], mat3[k * k];
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            tmp = fscanf(fp, "%d ", &mat1[k * i + j]);
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            tmp = fscanf(fp, "%d ", &mat2[k * i + j]);

    if (verbose == 1)
    {
        displaymat(mat1, k, "mat1");
        displaymat(mat2, k, "mat2");
    }

    gettimeofday(&t1, NULL);
    multiply(mat1, mat2, mat3, k);
    gettimeofday(&t2, NULL);

    printf("Time taken: %fms\n", (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_usec - t1.tv_usec) / 1000.0);

    if (verbose == 1)
        displaymat(mat3, k, "mat3");

    return 0;
}