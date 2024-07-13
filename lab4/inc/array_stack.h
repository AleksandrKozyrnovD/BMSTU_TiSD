#ifndef ARRAY_STACK_H__
#define ARRAY_STACK_H__

#include <stdlib.h>
#include "errors.h"
#include "oper.h"


typedef struct array_stack_t
{
    /*
     * Стак на массиве
     *
     * beg - Указатель на начало стака
     * curr - Указатель на текущий элемент стака
     * end - Указатель на законец стака
     * */
    operation_t *beg, *curr, *end;
} array_stack_t;

errors_t array_stack_init(array_stack_t *stack, int size);

void a_push(array_stack_t *stack, operation_t num);

operation_t a_pop(array_stack_t *stack);

operation_t a_peek(array_stack_t *stack);

void free_a_stack(array_stack_t *stack);

void a_print(FILE *file, array_stack_t *stack);

#endif

