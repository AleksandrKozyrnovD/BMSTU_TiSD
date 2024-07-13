#ifndef ERRORS_H__
#define ERRORS_H__

#include <stdio.h>

typedef enum errors_t
{
    EXIT_SUCCESSFUL,
    ERR_IO,
    ERR_RANGE,
    ERR_MEM,
    ERR_EMPTY,
    ERR_ARGS
} errors_t;

typedef enum str_valids_t
{
    VALID,
    TWO_OPERANDS,
    TWO_OPERATIONS,
    ONE_OPERATION,
    INVALID,
    NO_OPERAND
} str_valids_t;

void print_generic_valids(FILE *file, str_valids_t state);

#endif

