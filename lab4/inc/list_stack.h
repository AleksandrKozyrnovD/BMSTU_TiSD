#ifndef LIST_STACK_H__
#define LIST_STACK_H__

#include <stdio.h>
#include "oper.h"

typedef struct node_t node_t;
struct node_t
{
    /* Узел
     *
     *
     * prev - Указатель на предыдущий узел
     * */
    operation_t num;
    node_t *prev;
};

typedef struct listnode_t listnode_t;
struct listnode_t
{
    /* Узел
     *
     *
     * prev - Указатель на предыдущий узел
     * */
    operation_t *num;
    listnode_t *prev;
};

typedef struct list_stack_t
{
    /*
     * Стак на листе
     *
     * index - Размер текущего стака
     * top - Указатель на вершину (первый) узел стака
     * */
    int index;
    node_t *top;
} list_stack_t;

typedef struct free_t
{
    /*
     * Область освобожденных значений
     *
     * Односвязный список освобожденных областей памяти для стака на листе
     * Указатель на конечный узел здесь потому, что добавление конец занимает константное время
     * */
    listnode_t *head; //next == prev
    listnode_t *tail;
} free_t;

operation_t l_pop(list_stack_t *stack, free_t *area);

void l_push(list_stack_t *stack, operation_t num);

operation_t l_peek(list_stack_t *stack, free_t *area);

void free_l_stack(list_stack_t *stack, free_t *area);

void free_area(free_t *area);

void print_area(FILE *file, const free_t *area);

void l_print(FILE *file, list_stack_t *stack, free_t *area);

#endif

