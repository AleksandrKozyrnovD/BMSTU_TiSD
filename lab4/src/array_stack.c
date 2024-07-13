#include "array_stack.h"
#include "oper.h"
#include <stdlib.h>

errors_t array_stack_init(array_stack_t *stack, int size)
{
    stack->beg = malloc(size * sizeof(operation_t));
    if (!stack->beg)
        return ERR_MEM;
    stack->end = stack->beg + size;
    stack->curr = stack->beg;

    return EXIT_SUCCESSFUL;
}

void a_push(array_stack_t *stack, operation_t num)
{
    if (stack->curr == stack->end)
    {
        int size = stack->end - stack->beg;
        int newsize = size * 2;
        stack->beg = realloc(stack->beg, newsize * sizeof(operation_t));
        stack->curr = stack->beg + size;
        stack->end = stack->beg + newsize;
    }
    *(stack->curr) = num;
    stack->curr++;
}

operation_t a_pop(array_stack_t *stack)
{
    if (stack->curr == stack->beg)
        return -1;

    stack->curr--;
    return *(stack->curr);
}

operation_t a_peek(array_stack_t *stack)
{
    if (stack->curr == stack->beg)
        return -1;

    operation_t data = a_pop(stack);
    a_push(stack, data);

    return data;
}

void free_a_stack(array_stack_t *stack)
{
    free(stack->beg);
    stack->beg = NULL;
    stack->end = NULL;
    stack->curr = NULL;
}

void a_print(FILE *file, array_stack_t *stack)
{
    array_stack_t temp = { .beg = NULL, .curr = NULL };
    array_stack_init(&temp, stack->curr - stack->beg);
    operation_t data;
    void *ptr = NULL;
    if (stack->curr == stack->beg)
    {
        printf("Пусто\n");
        free(temp.beg);
        return;
    }

    printf("=/=/=/=/=/=/\n");
    while (stack->curr != stack->beg)
    {
        ptr = stack->curr;
        data = a_pop(stack);
        // fprintf(file, "Pointer: %p\nNumber/oper: %d\n", ptr, data);
        fprintf(file, "Ячейка памяти: %p\nЧисло/индекс операции: %d\n", ptr, data);
        printf("=/=/=/=/=/=/\n");
        a_push(&temp, data);
    }

    while (temp.beg != temp.curr)
        a_push(stack, a_pop(&temp));
    
    free(temp.beg);
}

