#ifndef __AVL_NODE_H__
#define __AVL_NODE_H__

#include <stddef.h>

typedef struct avl_node_t avl_node_t;

struct avl_node_t
{
    int num;
    size_t height;

    avl_node_t *left, *right;
    /*
        АВЛ-дерево.
        num - информационная часть
        height - высота данного узла
        left, right - правый и левый узлы
    */
};

/**
 * @brief Функция создания узла АВЛ-дерева
 *
 * @[in] param key Ключ
 * @param[in] num Число
 * @return NULL или созданный объект
 */
avl_node_t *avl_node_create(/* const char *key,  */int num);

/**
 * @brief Функция очистки узла
 *
 * @param[in,out] node Узел АВЛ-дерева. Он станет равным NULL
 */
void avl_node_free(avl_node_t **node);

void avl_node_print(avl_node_t *root, void *param);

#endif

