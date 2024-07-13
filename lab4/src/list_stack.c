#include "list_stack.h"
#include "oper.h"
#include <stdio.h>
#include <stdlib.h>

static void append(free_t *area, void *ptr)
{
    listnode_t *tmp = malloc(sizeof(listnode_t));
    tmp->prev = NULL;
    tmp->num = ptr;
    if (area->head == NULL)
    {
        area->head = tmp;
        area->tail = tmp;
    }
    else
    {
        area->tail->prev = tmp;
        area->tail = area->tail->prev;
    }
}

void l_push(list_stack_t *stack, operation_t num)
{
    node_t *node = malloc(sizeof(node_t));
    node->num = num;
    node->prev = stack->top;
    stack->top = node;
    stack->index++;
}

operation_t l_pop(list_stack_t *stack, free_t *area)
{
    if (stack->top == NULL)
        return 0;
    void *ptr = stack->top;
    node_t *tmp = stack->top;
    operation_t data = tmp->num;
    stack->top = stack->top->prev;
    
    append(area, ptr);
    free(tmp);
    
    stack->index--;
    return data;
}

operation_t l_peek(list_stack_t *stack, free_t *area)
{
    if (stack->top == NULL)
        return 0;

    operation_t data = l_pop(stack, area);
    l_push(stack, data);

    return data;
}

void free_l_stack(list_stack_t *stack, free_t *area)
{
    while (stack->top)
        l_pop(stack, area);
}

void free_area(free_t *area)
{
    if (area->head == NULL)
        return;
    listnode_t *prev = area->head;
    listnode_t *curr = prev->prev;
    while (curr)
    {
        free(prev);
        prev = curr;
        curr = curr->prev;
    }
    free(prev);
    area->head = NULL;
    area->tail = NULL;
}

void print_area(FILE *file, const free_t *area)
{
    printf("=/=/=/=/=/=/\n");
    for (listnode_t *curr = area->head; curr != NULL; curr = curr->prev)
    {
        fprintf(file, "Ячейка памяти: %p\n", (void *) curr->num);
        printf("=/=/=/=/=/=/\n");
    }
}

void l_print(FILE *file, list_stack_t *stack, free_t *area)
{
    list_stack_t temp = { .top = NULL, .index = 0 };
    free_t tmp = { .head = NULL, .tail = NULL };
    operation_t data;
    void *ptr = NULL;
    if (stack->top == NULL)
    {
        printf("Пусто\n");
        return;
    }

    printf("=/=/=/=/=/=/\n");
    while (stack->top)
    {
        ptr = stack->top;
        data = l_pop(stack, area);
        fprintf(file, "Ячейка памяти: %p\nЧисло/индекс оператора: %d\n", ptr, data);
        printf("=/=/=/=/=/=/\n");
        l_push(&temp, data);
    }

    while (temp.top)
        l_push(stack, l_pop(&temp, &tmp));

    free_area(&tmp);
}

