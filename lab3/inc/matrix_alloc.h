#ifndef MATRIX_ALLOC_H__
#define MATRIX_ALLOC_H__

#include "matrix.h"
#include "errors.h"

errors_t sparse_matrix_alloc(spr_matrix_t *matrix, int rows, int cols);

errors_t std_matrix_alloc(std_matrix_t *matrix, int rows, int cols);

void free_sparse_matrix(spr_matrix_t *matrix);

void free_std_matrix(std_matrix_t *matrix);

#endif

