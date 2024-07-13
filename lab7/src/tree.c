#include <stdio.h>
#include <stdlib.h>

#include "tree.h"


tree_node_t *create_node(/*int depth, tree_node_t *parent, tree_node_t *left, tree_node_t *right, */int value, char option)
{
    tree_node_t *new_node = malloc(sizeof(tree_node_t));
    if (new_node == NULL)
        printf("Ошибка выделения памяти для узла дерева!\n");

    // node_init(new_node, depth, parent, left, right, value, option);
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->value = value;
    new_node->option = option;

    return new_node;
}

tree_node_t *create_tree(int arr[9])
{
    tree_node_t *plus1 = create_node(/*0, NULL, NULL, NULL, */0, '+');
    tree_node_t *a = create_node(/*1, plus1, NULL, NULL, */arr[0], ' ');
    tree_node_t *multi1 = create_node(/* 1, plus1, NULL, NULL,  */0, '*');
    plus1->left = a;
    plus1->right = multi1;

    // printf("plus1: %p / %p\n\n", plus1->left, plus1->right);

    tree_node_t *b = create_node(/* 2, multi1, NULL, NULL,  */arr[1], ' ');
    tree_node_t *plus2 = create_node(/* 2, multi1, NULL, NULL,  */0, '+');
    multi1->left = b;
    multi1->right = plus2;

    // printf("multi1: %p / %p\n\n", multi1->left, multi1->right);

    tree_node_t *minus1 = create_node(/* 3, plus2, NULL, NULL,  */0, '-');
    tree_node_t *multi2 = create_node(/* 3, plus2, NULL, NULL,  */0, '*');
    plus2->left = minus1;
    plus2->right = multi2;

    // printf("plus2: %p / %p\n\n", plus2->left, plus2->right);
    // printf("multi2 value / option = %d / %c\n\n", plus2->right->value, plus2->right->option);


    tree_node_t *plus3 = create_node(/* 4, minus1, NULL, NULL,  */0, '+');
    tree_node_t *minus2 = create_node(/* 4, minus1, NULL, NULL,  */0, '-');
    minus1->left = plus3;
    minus1->right = minus2;

    // printf("minus1: %p / %p\n\n", minus1->left, minus1->right);


    tree_node_t *d = create_node(/* 4, multi2, NULL, NULL,  */arr[3], ' ');
    tree_node_t *plus4 = create_node(/* 4, multi2, NULL, NULL,  */0, '+');
    multi2->left = d;
    multi2->right = plus4;

    // printf("multi2: %p / %p\n\n", multi2->left, multi2->right);

    tree_node_t *c = create_node(/* 5, plus3, NULL, NULL,  */arr[2], ' ');
    tree_node_t *i = create_node(/* 5, plus3, NULL, NULL,  */arr[8], ' ');
    plus3->left = c;
    plus3->right = i;

    tree_node_t *e = create_node(/* 5, plus4, NULL, NULL,  */arr[4], ' ');
    tree_node_t *f = create_node(/* 5, plus4, NULL, NULL,  */arr[5], ' ');
    plus4->left = e;
    plus4->right = f;

    tree_node_t *g = create_node(/* 5, minus2, NULL, NULL,  */arr[6], ' ');
    tree_node_t *h = create_node(/* 5, minus2, NULL, NULL,  */arr[7], ' ');
    minus2->left = g;
    minus2->right = h;

    return plus1;
}

void print_tree(tree_node_t *root, int place)
{
    int add_space = 6;

    place += add_space;

    if (root == NULL)
        return;

    // printf("pointer = %p\n", root);
    // printf("value / option = %d / %c\n\n", root->value, root->option);

    if (root->right != NULL)
        print_tree(root->right, place);

    for (int i = add_space; i < place; i++)
        printf(" ");

    if (root->option != ' ')
        printf("{ %c }\n", root->option);
    else
        printf("{ %d }\n", root->value);

    if (root->left != NULL)
        print_tree(root->left, place);
}

void node_clear(tree_node_t *node)
{
    free(node);
}

void node_print(tree_node_t *node)
{
    if (node->option == ' ')
        printf("%d ", node->value);
    else
        printf("%c ", node->option);

}

void node_print_int(tree_node_t *node)
{
    if (node->option != ' ')
        printf("%d ", node->value);
}

void prefix(tree_node_t *root, node_tree_func_t func)
{
    if (!root)
        return;

    func(root);
    prefix(root->left, func);
    prefix(root->right, func);
}

void prefix_param(tree_node_t *root, void (*func)(tree_node_t *, void *), void *param)
{
    if (!root)
        return;

    func(root, param);
    prefix_param(root->left, func, param);
    prefix_param(root->right, func, param);
}

void infix(tree_node_t *root, node_tree_func_t func)
{
    if (!root)
        return;
    
    infix(root->left, func);
    func(root);
    infix(root->right, func);
}

void infix_param(tree_node_t *root, void (*func)(tree_node_t *, void *), void *param)
{
    if (!root)
        return;

    infix_param(root->left, func, param);
    func(root, param);
    infix_param(root->right, func, param);
}

void postfix(tree_node_t *root, node_tree_func_t func)
{
    if (!root)
        return;
    
    postfix(root->left, func);
    postfix(root->right, func);
    func(root);
}

void postfix_zad(tree_node_t *root, int mode)
{
    if (root != NULL)
    {
        postfix_zad(root->left, mode);
        postfix_zad(root->right, mode);

        if (root->option == '+')
            root->value = root->left->value + root->right->value;
        else if (root->option == '-')
            root->value = root->left->value - root->right->value;
        else if (root->option == '*')
            root->value = root->left->value * root->right->value;

        if (mode && root->option == ' ')
            printf("%d ", root->value);

        if (mode && root->option != ' ')
            printf("%c ", root->option);
    }
}

tree_node_t *add_to_tree(tree_node_t *root, tree_node_t *new_node)
{
    if (!root)
    {
        // printf("Вставка сюда\n");
        return new_node;
    }

    int score = new_node->value - root->value;

    if (!score)
        return root;
    
    if (score < 0)
    {
        // printf("Добавление влево\n");
        root->left = add_to_tree(root->left, new_node);
    }
    else
    {
        // printf("Добавление вправо\n");
        root->right = add_to_tree(root->right, new_node);
    }

    return root;
}

tree_node_t *find_in_tree(tree_node_t *root, int num)
{
    if (!root)
        return NULL;

    int score = num - root->value;

    if (!score)
        return root;

    if (score < 0)
        return find_in_tree(root->left, num);
    else
        return find_in_tree(root->right, num);

    return NULL;
}

tree_node_t *find_in_tree_zad(tree_node_t *root, int num, int *count)
{
    if (!root)
        return NULL;

    int score = num - root->value;
    (*count)++;
    if (!score)
        return root;

    if (score < 0)
        return find_in_tree_zad(root->left, num, count);
    else
        return find_in_tree_zad(root->right, num, count);

    return NULL;
}

static int is_leaf(tree_node_t *root)
{
    return !root->right && !root->left;
}

static int is_full(tree_node_t *root)
{
    return root->left && root->right;
}

tree_node_t *remove_in_tree(tree_node_t *root, int num)
{
    if (!root)
        return NULL;

    int score = num - root->value;

    if (!score)
    {
        if (is_leaf(root))
        {
            node_clear(root);
            return NULL;
        }
        else if (!is_full(root)) //1 потомок
        {
            tree_node_t *tmp = NULL;
            if (root->right)
            {
                tmp = root->right;
                node_clear(root);
                return tmp;
            }
            else
            {
                tmp = root->left;
                node_clear(root);
                return tmp;
            }
        }
        else
        {
            /*Найдем самого левого потомка правого поддерева*/
            tree_node_t *leftmost = root->right;
            while (leftmost->left)
                leftmost = leftmost->left;
            root->value = leftmost->value;
            root->right = remove_in_tree(root->right, root->value); //Удаляем этот узел
            return root;
        }
    }

    if (score < 0)
        root->left = remove_in_tree(root->left, num);
    else
        root->right = remove_in_tree(root->right, num);

    return root;
}

void to_dot(tree_node_t *root, void *param)
{
    if (!root)
        return;

    FILE *file = param;
    if (root->left)
    {
        if (root->option == ' ')
        {
            if (root->left->option == ' ')
                fprintf(file, "%d -> %d;\n", root->value, root->left->value);
            else
                fprintf(file, "%d -> \"%c\";\n", root->value, root->left->option);
        }
        else
            fprintf(file, "\"%c\" -> %d;\n", root->option, root->left->value);
    }
    if (root->right)
    {
        if (root->option == ' ')
            fprintf(file, "%d -> %d;\n", root->value, root->right->value);
        else
            fprintf(file, "\"%c\" -> %d;\n", root->option, root->right->value);
    }
}


void export_to_dot(FILE *f, const char *tree_name, tree_node_t *root)
{
    if (!root)
        return;
    fprintf(f, "digraph %s {\n", tree_name);
    prefix_param(root, to_dot, f);
    fprintf(f, "}\n");
}

