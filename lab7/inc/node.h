#ifndef __LIST_NODE_H__
#define __LIST_NODE_H__

#include <stdio.h>

typedef struct node list_node_t;

struct node
{
    int data;
    list_node_t *next;
};

/**
 * @brief Создание узла. Динамически выделяет память на его структуру, но не выделяет память под информационную часть
 *
 * @param[in] data Указатель на информационную часть
 * @return Указатель на узел
 */
list_node_t *list_node_create(int data/* , char *key */);

/**
 * @brief Освобобождение памяти из-поз узла
 *
 * @param[in] node Указатель на узел
 */
void list_node_free(list_node_t *node);

/**
 * @brief Функция печати узла (в данном случае студентов)
 *
 * @param[in] file Указатель на файл
 * @param[in] node Указатель на узел
 */
void list_node_print(FILE *file, const list_node_t *node);

/**
 * @brief Функция сравнения узлов (???)
 *
 * @param[in] left Указатель на левый
 * @param[in] right Указатель на правый
 * @return Разница целочисленная
 */
int list_node_cmp(const void *left, const void *right);

#endif
