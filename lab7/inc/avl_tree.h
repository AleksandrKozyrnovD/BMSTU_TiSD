#ifndef __TREE_H__
#define __TREE_H__

#include <stdio.h>
#include <stdbool.h>
#include "avl_node.h"

avl_node_t *tree_insert(avl_node_t *root/* , const char *key */, int num);

avl_node_t *tree_delete(avl_node_t *root, int num/*,  const char *key */);

avl_node_t *tree_search(avl_node_t *root, int num/* , const char *key */);

avl_node_t *tree_search_zad(avl_node_t *root, int num, int *count/* , const char *key */);

void inorder_param(avl_node_t *root, void (*action)(avl_node_t *, void *param), void *param);

void preorder_param(avl_node_t *root, void (*action)(avl_node_t *, void *param), void *param);

void postorder_param(avl_node_t *root, void (*action)(avl_node_t *, void *param), void *param);

void avl_to_dot(avl_node_t *root, void *param);

void avl_export_to_dot(FILE *file, const char *treename, avl_node_t *root);

#endif


