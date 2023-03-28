#include "shared.h"

// Matrix structure to hold 2D arrays
typedef struct matrix
{
    int rows;
    int cols;
    int **data;
} matrix;

void displayMat(matrix *mat, char *matrixName)
{
    printf("%s:\n", matrixName);
    for (int i = 0; i < mat->rows; i++)
    { // iterate over rows
        for (int j = 0; j < mat->cols; j++)
        {
            printf("\t%d", mat->data[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < mat->cols; i++)
        printf("\t");
    printf("\t(%dx%d)\n", mat->rows, mat->cols);
}

// Allocate memory for a new matrix
matrix *new_matrix(int rows, int cols)
{
    matrix *m = (matrix *)malloc(sizeof(matrix));
    m->rows = rows;
    m->cols = cols;
    m->data = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        m->data[i] = (int *)malloc(cols * sizeof(int));
    }
    return m;
}

// Free memory of a matrix
void free_matrix(matrix *m)
{
    for (int i = 0; i < m->rows; i++)
    {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}

// Initialize a matrix with data
void init_matrix(matrix *m, int *data)
{
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            m->data[i][j] = data[m->rows * i + j];
        }
    }
}

// divide matrix into 4 even submatrices
void divide(matrix *m, matrix *submatrices[4])
{
    int k = m->rows / 2;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            submatrices[2 * i + j] = new_matrix(k, k);
            for (int p = 0; p < k; p++)
            {
                for (int q = 0; q < k; q++)
                {
                    submatrices[2 * i + j]->data[p][q] = m->data[i * k + p][j * k + q];
                }
            }
        }
    }
}

// Combine 4 submatrices into a single matrix
void combine(matrix *m, matrix *submatrices[4])
{
    int k = submatrices[0]->rows;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int p = 0; p < k; p++)
            {
                for (int q = 0; q < k; q++)
                {
                    m->data[i * k + p][j * k + q] = submatrices[2 * i + j]->data[p][q];
                }
            }
            free_matrix(submatrices[2 * i + j]);
        }
    }
}

matrix *add(matrix *a, matrix *b)
{
    int rows = a->rows;
    int cols = a->cols;
    matrix *result = new_matrix(rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }

    return result;
}

matrix *subtract(matrix *a, matrix *b)
{
    int rows = a->rows;
    int cols = a->cols;
    matrix *result = new_matrix(rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }

    return result;
}

matrix *strassen(matrix *a, matrix *b)
{
    int n = a->rows;
    if (n == 1)
    {
        matrix *c = new_matrix(1, 1);
        c->data[0][0] = a->data[0][0] * b->data[0][0];
        return c;
    }

    int k = a->rows / 2;
    matrix *submatricesA[4];
    matrix *submatricesB[4];
    matrix *intermediateS[10];
    matrix *submatricesP[7];
    matrix *submatricesC[4];

    // Divide matrices A and B into 4 submatrices each
    divide(a, submatricesA);
    divide(b, submatricesB);

    intermediateS[0] = subtract(submatricesB[1], submatricesB[3]);
    intermediateS[1] = add(submatricesA[0], submatricesA[1]);
    intermediateS[2] = add(submatricesA[2], submatricesA[3]);
    intermediateS[3] = subtract(submatricesB[2], submatricesB[0]);
    intermediateS[4] = add(submatricesA[0], submatricesA[3]);
    intermediateS[5] = add(submatricesB[0], submatricesB[3]);
    intermediateS[6] = subtract(submatricesA[1], submatricesA[3]);
    intermediateS[7] = add(submatricesB[2], submatricesB[3]);
    intermediateS[8] = subtract(submatricesA[0], submatricesA[2]);
    intermediateS[9] = add(submatricesB[0], submatricesB[1]);

    // Calculate 7 submatrices
    submatricesP[0] = strassen(submatricesA[0], intermediateS[0]);
    submatricesP[1] = strassen(intermediateS[1], submatricesB[3]);
    submatricesP[2] = strassen(intermediateS[2], submatricesB[0]);
    submatricesP[3] = strassen(submatricesA[3], intermediateS[3]);
    submatricesP[4] = strassen(intermediateS[4], intermediateS[5]);
    submatricesP[5] = strassen(intermediateS[6], intermediateS[7]);
    submatricesP[6] = strassen(intermediateS[8], intermediateS[9]);

    // Combine submatrices C into a single matrix C
    matrix *c = new_matrix(2 * k, 2 * k);
    submatricesC[0] = add(subtract(add(submatricesP[4], submatricesP[3]), submatricesP[1]), submatricesP[5]);
    submatricesC[1] = add(submatricesP[0], submatricesP[1]);
    submatricesC[2] = add(submatricesP[2], submatricesP[3]);
    submatricesC[3] = subtract(add(submatricesP[0], submatricesP[4]), add(submatricesP[2], submatricesP[6]));

    combine(c, submatricesC);

    for (int i = 0; i < 10; i++)
        free_matrix(intermediateS[i]);
    for (int i = 0; i < 7; i++)
        free_matrix(submatricesP[i]);
    for (int i = 0; i < 4; i++)
        free_matrix(submatricesA[i]);
    for (int i = 0; i < 4; i++)
        free_matrix(submatricesB[i]);
    return c;
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
    int mat1[k * k], mat2[k * k];
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            tmp = fscanf(fp, "%d ", &mat1[k * i + j]);
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            tmp = fscanf(fp, "%d ", &mat2[k * i + j]);

    matrix *Mat1 = new_matrix(k, k);
    matrix *Mat2 = new_matrix(k, k);
    init_matrix(Mat1, mat1);
    init_matrix(Mat2, mat2);

    if (verbose == 1)
    {
        displayMat(Mat1, "mat1");
        displayMat(Mat2, "mat2");
    }

    gettimeofday(&t1, NULL);
    // multiply(mat1, mat2, mat3, k);
    matrix *Mat3 = strassen(Mat1, Mat2);
    gettimeofday(&t2, NULL);

    printf("Time taken: %fms\n", (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_usec - t1.tv_usec) / 1000.0);

    if (verbose == 1)
        displayMat(Mat3, "mat3");

    free_matrix(Mat1);
    free_matrix(Mat2);
    free_matrix(Mat3);
    return 0;
}