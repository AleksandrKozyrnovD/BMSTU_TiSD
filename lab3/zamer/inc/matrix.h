#ifndef MATRIX_H__
#define MATRIX_H__

#include <stdio.h>
#include <stddef.h>
#include "errors.h"

typedef struct spr_matrix_t
{
    int rows, cols, size;
    //size - count of non-zero elems

    int *A;
    int *JA;
    int *IA;
    //A - values
    //JA - rows of value i
    //IA compressed rows
} spr_matrix_t;

typedef struct std_matrix_t
{
    int rows, cols;

    int **A;
} std_matrix_t;

errors_t set_sparse_matrix(spr_matrix_t *matrix, int row, int col, int value);

errors_t clear_zeros(spr_matrix_t *matrix);

void dump_sparse_matrix(FILE *file, const spr_matrix_t *matrix);

void dump_std_matrix(FILE *file, const std_matrix_t *matrix);

void read_file(FILE *file, spr_matrix_t *A, std_matrix_t *B);

unsigned long long microseconds_now(void);

#endif

