#include "node.h"
#include <stdlib.h>
#include <string.h>

list_node_t *list_node_create(int data/* , char *key */)
{
	list_node_t *tmp = malloc(sizeof(list_node_t));
	if (!tmp)
	{
		return NULL;
	}
	tmp->next = NULL;
	tmp->data = data;
	// tmp->key = key;

	return tmp;
}

void list_node_free(list_node_t *node)
{
	if (!node)
	{
		return;
	}
	node->next = NULL;
	node->data = 0;
	// free(node->key);
	free(node);
}

int list_node_cmp(const void *left, const void *right)
{
	// return strcmp(((list_node_t *) left)->key, ((list_node_t *) right)->key);
    return ((list_node_t *) left)->data - ((list_node_t *) right)->data;
}
