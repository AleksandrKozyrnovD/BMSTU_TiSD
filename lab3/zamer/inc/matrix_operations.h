#ifndef MATRIX_OPERATIONS_H__
#define MATRIX_OPERATIONS_H__

#include "matrix.h"
#include "errors.h"
#include "matrix_alloc.h"

errors_t add_sparse_matrix(const spr_matrix_t *left, const spr_matrix_t *right, spr_matrix_t *result);

errors_t add_std_matrix(const std_matrix_t *left, const std_matrix_t *right, std_matrix_t *result);

#endif

