#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../inc/matrix.h"
#include "../inc/matrix_alloc.h"
#include "../inc/errors.h"
#include "../inc/matrix_operations.h"


int main (int argc, char **argv)
{
    //file1 file2 fileout1 fileout2
    if (argc != 5)
        return ERR_IO;

    FILE *file1, *file2, *fileout1, *fileout2;

    file1 = fopen(argv[1], "r");
    file2 = fopen(argv[2], "r");
    fileout1 = fopen(argv[3], "a");
    fileout2 = fopen(argv[4], "a");

    std_matrix_t A, B, C;
    spr_matrix_t D, E, F;

    A.A = NULL;
    B.A = NULL;
    C.A = NULL;

    D.A = NULL;
    D.JA = NULL;
    D.IA = NULL;
    E.A = NULL;
    E.JA = NULL;
    E.IA = NULL;
    F.A = NULL;
    F.JA = NULL;
    F.IA = NULL;
    

    read_file(file1, &D, &A);
    read_file(file2, &E, &B);
    
    F.A = (int *)malloc(sizeof(int) * (D.size + E.size));
    F.JA = malloc(sizeof(int) * (D.size + E.size));
    F.IA = malloc(sizeof(int) * (D.rows + 1));
    std_matrix_alloc(&C, A.rows, A.cols);

    // sparse_matrix_alloc(&F, A.rows, A.cols);
    unsigned long long beg, end;

    beg = microseconds_now();
    add_sparse_matrix(&D, &E, &F);
    end = microseconds_now();
    // dump_sparse_matrix(stdout, &F);
    free_sparse_matrix(&F);

    fprintf(fileout1, "%llu\n", end - beg);

    beg = microseconds_now();
    add_std_matrix(&A, &B, &C);
    end = microseconds_now();
    // dump_std_matrix(stdout, &C);
    free_std_matrix(&C);

    fprintf(fileout2, "%llu\n", end - beg);

    fclose(file1);
    fclose(file2);
    fclose(fileout1);
    fclose(fileout2);
    free_sparse_matrix(&D);
    free_sparse_matrix(&E);
    free_sparse_matrix(&F);

    free_std_matrix(&A);
    free_std_matrix(&B);
    free_std_matrix(&C);

    return EXIT_SUCCESSFUL;
}
