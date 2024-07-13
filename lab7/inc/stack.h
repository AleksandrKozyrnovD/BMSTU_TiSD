#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>

typedef struct node_t node_t;
struct node_t
{
    /* Узел стека
     * prev - Указатель на предыдущий узел
     * type - целочисленное значение
     * */
    int type;
    node_t *prev;
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

/**
 * @brief Вывести из стека элемент
 *
 * @param[in,out] stack Стек
 * @param[out] type Число
 * @return Код ошибки
 */
int l_pop(list_stack_t *stack, int *type);

/**
 * @brief Вставить число в стек
 *
 * @param[in,out] stack Стек
 * @param[in] type Что вставляем
 * @return Код ошибки
 */
int l_push(list_stack_t *stack, int type);

/**
 * @brief Подсмотреть верхний элемент
 *
 * @param[in] stack Стек
 * @param[out] type Что подсматривается
 * @return Код ошибки
 */
int l_peek(list_stack_t *stack, int *type);

/**
 * @brief Освободить стек
 *
 * @param[in,out] stack Cтек
 */
void free_l_stack(list_stack_t *stack);

void l_print(FILE *file, list_stack_t *stack, const char *restrict format);

/**
 * @brief Посчитать выражение из задания
 *
 * @param arr Массив 9-ти чисел
 * @param NUMS_COUNT Константа (9)
 * @return Результат выражения
 */
int stack_get_result(int arr[9]);

#endif

