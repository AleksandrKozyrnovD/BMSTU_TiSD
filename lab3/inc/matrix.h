#ifndef MATRIX_H__
#define MATRIX_H__

#include <stdio.h>
#include <stddef.h>
#include "errors.h"

typedef struct spr_matrix_t
{
    // Разряженная матрица

    /*
     * rows - Количество строк в матрице
     * cols - Количество столбцов в матрице
     * size - Количество ненулевых элементов
     * */
    int rows, cols, size;

    /*
     * *A  - Вектор ненулевых значений. Его размер равен size
     * *JA - Вектор столбцов для ненулевых значений. Его размер равен size
     * *IA - Сжатый вектор строчек. Его размер rows + 1
     * */
    int *A, *JA, *IA;
} spr_matrix_t;

typedef struct std_matrix_t
{
    //Простая матрица

    /*
     * rows - Количество строк в матрице
     * cols - Количество строк в матрице
     * */
    int rows, cols;
    
    /*
     * **A - Матрица, представленная в виде указателей на массив, размером rows*cols, расположенный в одном месте
     * */
    int **A;
} std_matrix_t;

errors_t set_sparse_matrix(spr_matrix_t *matrix, int row, int col, int value);

errors_t clear_zeros(spr_matrix_t *matrix);

void dump_sparse_matrix(FILE *file, const spr_matrix_t *matrix);

void dump_std_matrix(FILE *file, const std_matrix_t *matrix);

unsigned long long microseconds_now(void);

#endif

