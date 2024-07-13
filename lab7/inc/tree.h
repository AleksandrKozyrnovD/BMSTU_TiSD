#ifndef __TREE__H__
#define __TREE__H__

#include <stdio.h>


/*typedef на узел дерева*/
typedef struct treenode tree_node_t;

struct treenode {
    tree_node_t *left;
    tree_node_t *right;
    int value;
    char option;
    /*
    left - Левый потомок текущего узла
    right - Правый потомок текущего узла
    value - числовое значение узла
    option - операция над числом (для вычисления выражения)
    */
};

typedef void (*node_tree_func_t)(tree_node_t *);

tree_node_t *create_node(int value, char option);

/**
 * @brief Напечатать дерево в консоль
 *
 * @param[in] root Указатель на корень дерева
 * @param[in] place Смещение (указывать 0 при вызове!)
 */
void print_tree(tree_node_t *root, int place);

/**
 * @brief Создать дерево выражения
 *
 * @param arr Массив 9-ти чисел
 * @param NUMS_COUNT Константа (9)
 * @return Узел на созданное дерево
 */
tree_node_t *create_tree(int arr[9]);

/**
 * @brief Префиксный обход дерева
 *
 * @param[in] root Указатель на корень дерева
 */
void prefix(tree_node_t *root, node_tree_func_t);

/**
 * @brief Префиксный обход дерева
 *
 * @param[in] root Указатель на корень дерева
 */
void prefix_param(tree_node_t *root, void (*)(tree_node_t *, void *param), void *param);

/**
 * @brief Префиксный обход дерева
 *
 * @param[in] root Указатель на корень дерева
 */
void infix_param(tree_node_t *root, void (*)(tree_node_t *, void *param), void *param);

/**
 * @brief Префиксный обход дерева по заданию
 *
 * @param[in] root Указатель на корень дерева
 */
void postfix_zad(tree_node_t *root, int mode);

/**
 * @brief Постфиксный обход дерева
 *
 * @param[in] root Указатель на корень дерева
 */
void postfix(tree_node_t *root, node_tree_func_t);

/**
 * @brief Инфиксный обход дерева
 *
 * @param[in] root Указатель на корень дерева
 */
void infix(tree_node_t *root, node_tree_func_t);

/**
 * @brief Создать узел дерева
 *
 * @param[in] value Числовое значение
 * @param[in] option Операция
 * @return Указатель на созданный узел (или NULL)
 */
tree_node_t *create_node(/*int depth, tree_node_t *parent, tree_node_t *left, tree_node_t *right, */int value, char option);

/**
 * @brief Добавить в дерево узел
 *
 * @param root Указатель на узел корня
 * @param new_node Указатель на вставляемый узел
 * @return Указатель на новый узел корня
 */
tree_node_t *add_to_tree(tree_node_t *root, tree_node_t *new_node);

/**
 * @brief Нахождение такого числа в дереве
 *
 * @param root Указатель на корень дерева
 * @param num Искомое число
 * @return Указатель на узел или NULL
 */
tree_node_t *find_in_tree(tree_node_t *root, int num);

tree_node_t *find_in_tree_zad(tree_node_t *root, int num, int *count);
/**
 * @brief Удалить из дерево искомое число
 *
 * @param root Указатель на корень дерева
 * @param num Удаляемое число
 * @return Указатель на новый корень
 */
tree_node_t *remove_in_tree(tree_node_t *root, int num);

/**
 * @brief Очистить узел дерева
 *
 * @param node Узел дерева
 */
void node_clear(tree_node_t *node);

void node_print(tree_node_t *node);

void node_print_int(tree_node_t *node);

/**
 * @brief Функция экспорта в .dot
 *
 * @param root Указатель на корень дерева (поддерева)
 * @param param Параметр
 */
void to_dot(tree_node_t *root, void *param);

/**
 * @brief По факту prefix'ный обход дерева. Создает файл .dot, который готов для graphviz.
 *
 * @details Вызывает функцию to_dot
 *
 * @param f Файловый указатель
 * @param tree_name Имя дерева
 * @param root Указатель на корень дерева
 */
void export_to_dot(FILE *f, const char *tree_name, tree_node_t *root);

#endif
