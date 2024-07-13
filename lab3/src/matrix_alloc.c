#include <stdlib.h>
#include "../inc/matrix_alloc.h"

void free_sparse_matrix(spr_matrix_t *matrix)
{
    free(matrix->A);
    free(matrix->IA);
    free(matrix->JA);
    matrix->A = NULL;
    matrix->JA = NULL;
    matrix->IA = NULL;

    matrix->rows = 0;
    matrix->cols = 0;
    matrix->size = 0;
}

errors_t sparse_matrix_alloc(spr_matrix_t *matrix, int rows, int cols)
{
    (void) cols;
    matrix->A = (int *)calloc(0, sizeof(int));
    if (!matrix->A)
        return ERR_MEM;

    matrix->JA = (int *)calloc(cols, sizeof(int));
    if (!matrix->JA)
    {
        free_sparse_matrix(matrix);
        return ERR_MEM;
    }
    matrix->IA = (int *)calloc(rows + 1, sizeof(int));
    if (!matrix->IA)
    {
        free_sparse_matrix(matrix);
        return ERR_MEM;
    }
    
    matrix->size = 0;
    matrix->cols = cols;
    matrix->rows = rows;

    return EXIT_SUCCESSFUL;
}

errors_t std_matrix_alloc(std_matrix_t *matrix, int rows, int cols)
{
    int **ptrs, *data;

    ptrs = malloc(rows * sizeof(int *));
    if (!ptrs)
        return ERR_MEM;
    data = calloc(rows * cols, sizeof(int));
    if (!data)
    {
        free(ptrs);
        return ERR_MEM;
    }
    
    *ptrs = data; //lsp ругается
    for (int i = 1; i < rows; i++)
        ptrs[i] = data + i * cols;

    matrix->A = ptrs;
    matrix->rows = rows;
    matrix->cols = cols;

    return EXIT_SUCCESSFUL;
}

void free_std_matrix(std_matrix_t *matrix)
{
    if (!matrix->A)
        return;
    int *min = matrix->A[0];

    for (int i = 1; i < matrix->rows; i++)
    {
        if (min > matrix->A[i])
            min = matrix->A[i];
    }

    free(min);
    free(matrix->A);
    matrix->A = NULL;
    matrix->rows = 0;
    matrix->cols = 0;
}
