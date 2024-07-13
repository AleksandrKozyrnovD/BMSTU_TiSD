// #define _GNU_SOURCE
// #include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "avl_node.h"


avl_node_t *avl_node_create(/* const char *key,  */int num)
{
    avl_node_t *node = malloc(sizeof(avl_node_t));
    if (!node)
        return NULL;

    // node->key = strdup(key);
    // if (!node->key)
    // {
    //     free(node);
    //     return NULL;
    // }
    node->num = num;
    node->left = NULL;
    node->right = NULL;
    node->height = 0;

    return node;
}

void avl_node_free(avl_node_t **node)
{
    // free((*node)->key);
    free(*node);
    *node = NULL;
}

void avl_node_print(avl_node_t *root, void *param)
{
    (void) param;
    printf("%d ", root->num);
}
