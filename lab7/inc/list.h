#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include "errors.h"
#include "node.h"

typedef void (*printnode)(FILE *, const list_node_t *);

typedef int (*comparator)(const void *, const void *);

/**
 * @brief Функция вставки нового узла в голову списка
 *
 * @param[in] head Указатель на голову списка
 * @param[in] node Указатель на вставляемый узел
 * @return Указатель на новую голову
 */
list_node_t *push_front(list_node_t *head, list_node_t *node);

/**
 * @brief Функция вставки нового узла в хвост списка
 *
 * @param[in] head Указатель на голову списка
 * @param[in] node Указатель на вставляемый узел
 * @return Указатель на новую голову
 */
list_node_t *push_end(list_node_t *head, list_node_t *node);

/**
 * @brief Функция удаления головного узла списка и возвращение его информационной части
 *
 * @param[in,out] head Указатель на голову
 * @return Указатель на информационную часть удаленного узла
 */
int pop_front(list_node_t **head);

/**
 * @brief Функция удаления хвостового узла списка и возвращение его информационной части
 *
 * @param[in,out] head Указатель на голову
 * @return Указатель на информационную часть удаленного узла
 */
int pop_back(list_node_t **head);

/**
 * @brief Функция вставки нового узла перед указанным
 *
 * @param[in,out] head Указатель на голову списка
 * @param[in] elem Указатель на вставляемый узел
 * @param[in] before Указатель на узел, перед которым нужно вставить узел
 */
void insert(list_node_t **head, list_node_t *elem, list_node_t *before);

/**
 * @brief Функция печати списка в файл
 *
 * @param[in] file Файловый указатель
 * @param[in] head Указатель на голову списка
 * @param[in] printnode Функция печати узла
 */
void list_print(FILE *file, const list_node_t *head, printnode);

/**
 * @brief Освобождение памяти из-под узлов (но не из-под их информационной части)
 *
 * @param[in] head Указатель на голову списка
 */
void list_free(list_node_t *head);

/**
 * @brief Функция копирования списка
 *
 * @param head Указатель на исходный головной узел списка
 * @param newhead Указатель на новый головной узел
 * @return Код ошибки
 */
errors_t list_copy(list_node_t *head, list_node_t **newhead);

/**
 * @brief Функция склеивания двух списков
 *
 * @param[in,out] head_a Указатель на голову списка, к которому присоединяется второй список
 * @param[in,out] head_b Указатель на присоединяемую голову второго списка
 */
void append(list_node_t **head_a, list_node_t **head_b);

void front_back_split(list_node_t *head, list_node_t **back);

list_node_t *sorted_merge(list_node_t **head_a, list_node_t **head_b, comparator cmp);

/**
 * @brief Функция сортировки слиянием
 *
 * @param[in] head Указатель на голову
 * @return Указатель на новую голову
 */
list_node_t *sort(list_node_t *head, comparator);

#endif
