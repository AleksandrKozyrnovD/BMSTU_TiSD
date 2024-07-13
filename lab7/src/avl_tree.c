#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_tree.h"

/**
 * @brief Функция нахождения высоты узла
 *
 * @param[in] root Узел дерева
 * @return Высота узла
 */
static size_t height(const avl_node_t *root)
{
    size_t left_h, right_h;

    if (!root)
        return 0;

    if (!root->left)
        left_h = 0;
    else
        left_h = 1 + root->left->height;

    if (!root->right)
        right_h = 0;
    else
        right_h = 1 + root->right->height;

    if (left_h > right_h)
        return left_h;
    return right_h;
}

/**
 * @brief Функция поворота трех узлов АВЛ-дерева влево
 *
 * @param[in] root Узел текущий
 * @return Новый узел
 */
static avl_node_t *rotate_left(avl_node_t *root)
{
    avl_node_t *right = root->right;
    if (!right)
        return root;
    root->right = right->left;
    right->left = root;

    root->height = height(root);
    right->height = height(right);
    
    return right;
}

/**
 * @brief Функция поворота трех узлов АВЛ-дерева вправо
 *
 * @param[in] root Узел текущий
 * @return Новый узел
 */
static avl_node_t *rotate_right(avl_node_t *root)
{
    avl_node_t *left= root->left;
    if (!left)
        return root;
    root->left = left->right;
    left->right = root;

    root->height = height(root);
    left->height = height(left);

    return left;
}

/**
 * @brief Балансирующий фактор. Проверяет разницу высот левого и правого поддеревьев
 *
 * @param[in] root Узел дерева
 * @return Фактор
 */
static int balance_factor(avl_node_t *root)
{
    int left_height, right_height;

    if (!root)
        return 0;

    if (!root->left)
        left_height = 0;
    else
        left_height = 1 + root->left->height;

    if (!root->right)
        right_height = 0;
    else
        right_height = 1 + root->right->height;

    return left_height - right_height;
}

avl_node_t *tree_insert(avl_node_t *root/* , const char *key */, int num)
{
    /* Если дерево пустое, то корень такой */
    if (root == NULL)
    {
        avl_node_t *newnode = avl_node_create(/* key,  */num);
        if (!newnode)
            return NULL;
        root = newnode;
    }
    else
    {
        /* Иначе ищем по дереву нужное место */
        // int cmp_score = strcmp(key, root->key);
        int cmp_score = num - root->num;
        if (cmp_score > 0)
        {
            /* Рекурсивно ищем нужный узел */
            root->right = tree_insert(root->right/* , key */, num);
            
            /* Балансируем дерево с помощью простых поворотов */
            if (balance_factor(root) == -2)
            {
                // if (strcmp(key, root->right->key) > 0)
                if (num - root->right->num > 0)
                    root = rotate_left(root);
                else
                {
                    root->right = rotate_right(root->right);
                    root = rotate_left(root);
                }
            }
        }
        else
        {
            /* Рекурсивно ищем нужный узел */
            root->left = tree_insert(root->left/* , key */, num);

            /* Балансируем дерево с помощью простых поворотов */
            if (balance_factor(root) == 2)
            {
                // if (strcmp(key, root->left->key) < 0)
                if (num - root->left->num < 0)
                    root = rotate_right(root);
                else
                {
                    root->left = rotate_left(root->left);
                    root = rotate_right(root);
                }
            }
        }
    }
    
    /* Новая высота узла */
    root->height = height(root);

    return root;
}

avl_node_t *tree_delete(avl_node_t *root, int num/* , const char *key */)
{
    // printf("Got %p\n", (void *) root);
    // char *keytmp = NULL;
    avl_node_t *temp = NULL;
    
    /* Если дерево пустое */
    if (!root)
    {
        // printf("NULL\n");
        return NULL;
    }

    // int cmp_score = strcmp(key, root->key);
    int cmp_score = num - root->num;
    
    /* Ищем наш узел */
    if (cmp_score > 0)
    {
        // printf("Remove right\n");
        root->right = tree_delete(root->right, num/*  key */);

        if (balance_factor(root) == 2)
        {
            if (balance_factor(root->left) >= 0)
                root = rotate_right(root);
            else
            {
                root->left = rotate_left(root->left);
                root = rotate_right(root);
            }
        }
    }
    else if (cmp_score < 0)
    {
        // printf("Remove left\n");
        root->left = tree_delete(root->left, num/*  key */);

        if (balance_factor(root) == -2)
        {
            if (balance_factor(root->right) >= 0)
                root = rotate_left(root);
            else
            {
                root->right = rotate_right(root->right);
                root = rotate_left(root);
            }
        }
    }
    else
    {
        // printf("Remove here\n");
        if (root->right)
        {
            // printf("Remove root->right\n");
            /* temp - самый левый потомок правого поддерева */
            temp = root->right;
            while (temp->left)
                temp = temp->left;
            
            /* Заменяем ключ с потомка */
            root->num = temp->num;
            
            /* Удалим этого потомка (по факту у нас к этому моменту два одинаковых узла, поэтому удаляем копию)*/
            // printf("find root->right with %d\n", temp->num);
            root->right = tree_delete(root->right, temp->num/* , temp->key */);

            if (balance_factor(root) == 2)
            {
                if (balance_factor(root->left) >= 0)
                    root = rotate_right(root);
                else
                {
                    root->left = rotate_left(root->left);
                    root = rotate_right(root);
                }
            }
            return root;
        }
        else
        {
            // printf("Root->right is NULL\n");
            /* Очищаем память под весь узел. Но возвращаем левый.
             *
             * Этот алгоритм предусматривает как узел с одним наследником, так и узел-лист.
             *
             * Этот else возникает, заметим, только в случае, когда правого поддерева нет,
             * то есть узел гарантировано с одним наследником или без.
             *
             * Если наследник есть, он возвращается, иначе NULL (по определению моего узла он либо NULL, либо указатель на узел).
             * */
            avl_node_t *temp = root->left;
            free(root);
            return temp;
            // free(root->key);
        }
    }
    
    // free(keytmp);
    root->height = height(root);
    
    return root;
}

avl_node_t *tree_search(avl_node_t *root, int num/* , const char *key */)
{
    if (!root)
        return NULL;

    // int cmp_score = strcmp(root->key, key);
    int cmp_score = root->num - num;
    
    if (cmp_score == 0)
        return root;
    else if (cmp_score > 0)
        return tree_search(root->left, num/* , key */);
    else
        return tree_search(root->right, num/* , key */);
}

avl_node_t *tree_search_zad(avl_node_t *root, int num, int *count/* , const char *key */)
{
    if (!root)
        return NULL;

    // int cmp_score = strcmp(root->key, key);
    int cmp_score = root->num - num;
    
    (*count)++;
    if (cmp_score == 0)
        return root;
    else if (cmp_score > 0)
        return tree_search_zad(root->left, num, count/* , key */);
    else
        return tree_search_zad(root->right, num, count/* , key */);
}

void inorder_param(avl_node_t *root, void (*action)(avl_node_t *, void *), void *param)
{
    if (!root)
        return;

    inorder_param(root->left, action, param);
    // action(root->key, &root->num, param);
    action(root, param);
    inorder_param(root->right, action, param);
}

void preorder_param(avl_node_t *root, void (*action)(avl_node_t *, void *), void *param)
{
    if (!root)
        return;

    // action(root->key, &root->num, param);
    action(root, param);
    preorder_param(root->left, action, param);
    preorder_param(root->right, action, param);
}

void postorder_param(avl_node_t *root, void (*action)(avl_node_t *, void *), void *param)
{
    if (!root)
        return;

    postorder_param(root->left, action, param);
    postorder_param(root->right, action, param);
    // action(root->key, &root->num, param);
    action(root, param);
}

void avl_to_dot(avl_node_t *root, void *param)
{
    if (!root)
        return;
    
    FILE *file = param;
    if (root->left)
    {
        fprintf(file, "%d -> %d;\n", root->num, root->left->num);
    }
    if (root->right)
    {
        fprintf(file, "%d -> %d;\n", root->num, root->right->num);
    }
}


void avl_export_to_dot(FILE *f, const char *tree_name, avl_node_t *root)
{
    if (!root)
        return;
    fprintf(f, "digraph %s {\n", tree_name);
    preorder_param(root, avl_to_dot, f);
    fprintf(f, "}\n");
}

