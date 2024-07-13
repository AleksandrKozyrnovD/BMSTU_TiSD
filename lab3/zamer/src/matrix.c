#include "../inc/matrix.h"
#include "../inc/matrix_alloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


errors_t set_sparse_matrix(spr_matrix_t *matrix, int row, int col, int value)
{
    // printf("Rows = %d, cols = %d\n", matrix->rows, matrix->cols);
    if (row < 0 || row >= matrix->rows || col < 0 || col >= matrix->cols)
        return ERR_RANGE;
    
    // printf("First check passed\n");
    int index = -1;
    for (int i = matrix->IA[row]; i < matrix->IA[row + 1]; i++)
    {
        if (matrix->JA[i] == col)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
        matrix->A[index] = value;
    else
    {
        int *NewA, *NewJA, NewSize;
        NewSize = matrix->size + 1;
        NewA = realloc(matrix->A, NewSize * sizeof(int));
        if (!NewA)
            return ERR_MEM;
        NewJA = realloc(matrix->JA, NewSize * sizeof(int));
        if (!NewJA)
        {
            free(NewA);
            return ERR_MEM;
        }
        matrix->A = NewA;
        matrix->JA = NewJA;
        matrix->size = NewSize;

        for (int i = NewSize - 1; i > matrix->IA[row]; i--)
        {
            matrix->A[i] = matrix->A[i - 1];
            matrix->JA[i] = matrix->JA[i - 1];
        }

        matrix->A[matrix->IA[row]] = value;
        matrix->JA[matrix->IA[row]] = col;

        for (int i = row + 1; i <= matrix->rows; i++)
            matrix->IA[i]++;
    }
    
    return EXIT_SUCCESSFUL;
}


void dump_sparse_matrix(FILE *file, const spr_matrix_t *matrix)
{
    int value;
    fprintf(file, "______________________________\n");
    fprintf(file, "A  = { ");
    for (int i = 0; i < matrix->size; i++)
        fprintf(file, "%d ", matrix->A[i]);
    fprintf(file, "}\n");
    fprintf(file, "JA = { ");
    for (int i = 0; i < matrix->size; i++)
        fprintf(file, "%d ", matrix->JA[i]);
    fprintf(file, "}\n");
    fprintf(file, "IA = { ");
    for (int i = 0; i <= matrix->rows; i++)
        fprintf(file, "%d ", matrix->IA[i]);
    fprintf(file, "}\n\n");
    
    if (matrix->rows >= 50 || matrix->cols >= 50)
    {
        fprintf(file, "\n______________________________\n");
        return;
    }
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
        {
            value = 0;
            for (int k = matrix->IA[i]; k < matrix->IA[i + 1]; k++)
                if (matrix->JA[k] == j)
                {
                    value = matrix->A[k];
                    break;
                }
            fprintf(file, "%5d ", value);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n______________________________\n");
}

void dump_std_matrix(FILE *file, const std_matrix_t *matrix)
{
    fprintf(file, "______________________________\n");
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
            fprintf(file, "%5d ", matrix->A[i][j]);
        fprintf(file, "\n");
    }
    fprintf(file, "\n______________________________\n");
}

errors_t clear_zeros(spr_matrix_t *matrix)
{
    int nonzeros = 0;

    int *ZeroA = malloc(matrix->size * sizeof(int));
    if (!ZeroA)
        return ERR_MEM;
    int *ZeroJA = malloc(matrix->size * sizeof(int));
    if (!ZeroJA)
    {
        free(ZeroA);
        return ERR_MEM;
    }
    int *ZeroAI = malloc((matrix->rows + 1) * sizeof(int));
    if (!ZeroAI)
    {
        free(ZeroA);
        free(ZeroJA);
        return ERR_MEM;
    }

    for (int i = 0; i < matrix->rows; i++)
    {
        ZeroAI[i] = nonzeros;

        for (int j = matrix->IA[i]; j < matrix->IA[i + 1]; j++)
        {
            int value = matrix->A[j];
            if (value)
            {
                ZeroA[nonzeros] = value;
                ZeroJA[nonzeros] = matrix->JA[j];
                nonzeros++;
            }
        }
    }

    int rows = matrix->rows;
    int cols = matrix->cols;
    ZeroAI[rows] = nonzeros;
    free_sparse_matrix(matrix);
    matrix->rows = rows;
    matrix->cols = cols;


    matrix->A = ZeroA;
    matrix->JA = ZeroJA;
    matrix->IA = ZeroAI;
    matrix->size = nonzeros;

    return EXIT_SUCCESSFUL;
}

unsigned long long microseconds_now(void)
{
    struct timeval value;

    if (gettimeofday(&value, NULL))
        return (unsigned long long) -1;
    
    return value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

void read_file(FILE *file, spr_matrix_t *A, std_matrix_t *B)
{
    rewind(file);
    int rows, cols, count, value;

    fscanf(file, "%d %d %d", &rows, &cols, &count);

    // printf("%d %d %d\n", rows, cols, count);

    // free_sparse_matrix(A);
    // free_std_matrix(B);

    sparse_matrix_alloc(A, rows, cols);
    std_matrix_alloc(B, rows, cols);

    // printf("==============\n");
    for (int i = 0; i < count; i++)
    {
        fscanf(file, "%d %d %d", &rows, &cols, &value);
        // printf("%d %d %d\n", rows, cols, value);
        set_sparse_matrix(A, rows, cols, value);
        B->A[rows][cols] = value;
    }
    // printf("==============\n");
}
