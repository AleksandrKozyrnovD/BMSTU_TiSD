#include "../inc/matrix_operations.h"
#include <stdlib.h>

errors_t add_sparse_matrix(const spr_matrix_t *left, const spr_matrix_t *right, spr_matrix_t *result)
{
    // Check if the dimensions of the matrices are compatible
    if (left->rows != right->rows || left->cols != right->cols)
    {
        return ERR_RANGE; // Return 0 to indicate failure
    }

    // Initialize the result matrix with the same dimensions
    result->rows = left->rows;
    result->cols = left->cols;
    result->size = 0; // Initialize the size of the result matrix to 0
    //
    // Allocate memory for the result matrix arrays
    // result->A = (int *)malloc(sizeof(int) * (left->size + right->size));
    // if (!result->A)
    //     return ERR_MEM;
    // result->JA = (int *)malloc(sizeof(int) * (left->size + right->size));
    // if (!result->JA)
    // {
    //     free_sparse_matrix(result);
    //     return ERR_MEM;
    // }
    // result->IA = (int *)malloc(sizeof(int) * (left->rows + 1));
    // if (!result->IA)
    // {
    //     free_sparse_matrix(result);
    //     return ERR_MEM;
    // }

    // Perform the addition operation
    int i = 0, j = 0, k = 0;

    for (int row = 0; row < left->rows; row++)
    {
        result->IA[row] = k;

        while (i < left->IA[row + 1] && j < right->IA[row + 1])
        {
            if (left->JA[i] < right->JA[j])
            {
                result->A[k] = left->A[i];
                result->JA[k] = left->JA[i];
                i++;
            }
            else if (left->JA[i] > right->JA[j])
            {
                result->A[k] = right->JA[j];
                result->JA[k] = right->JA[j];
                j++;
            }
            else
            {
                result->A[k] = left->A[i] + right->A[j];
                result->JA[k] = left->JA[i];
                i++;
                j++;
            }
            k++;
        }

        while (i < left->IA[row + 1])
        {
            result->A[k] = left->A[i];
            result->JA[k] = left->JA[i];
            i++;
            k++;
        }
        while (j < right->IA[row + 1])
        {
            result->A[k] = right->A[j];
            result->JA[k] = right->JA[j];
            j++;
            k++;
        }
    }

    result->IA[result->rows] = k;
    result->size = k;


    return EXIT_SUCCESSFUL; // Return 1 to indicate success
}

errors_t add_std_matrix(const std_matrix_t *left, const std_matrix_t *right, std_matrix_t *result)
{
    if (left->rows != right->rows || left->cols != right->cols)
    {
        return ERR_RANGE; // Return 0 to indicate failure
    }
    
    // errors_t error = std_matrix_alloc(result, left->rows, left->cols);
    // if (error != EXIT_SUCCESSFUL)
    //     return error;

    for (int i = 0; i < left->rows; i++)
    {
        for (int j = 0; j < left->cols; j++)
            result->A[i][j] = left->A[i][j] + right->A[i][j];
    }

    return EXIT_SUCCESSFUL;
}

