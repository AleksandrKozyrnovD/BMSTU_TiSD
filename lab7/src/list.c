#include "list.h"
#include "errors.h"
#include "node.h"

list_node_t *push_front(list_node_t *head, list_node_t *node)
{
	if (!head || !node)
	{
		return node;
	}

	node->next = head;

	return node;
}

list_node_t *push_end(list_node_t *head, list_node_t *node)
{
	if (!head)
	{
		return node;
	}

	list_node_t *tmp = head;
	while (tmp->next)
	{
		tmp = tmp->next;
	}
	tmp->next = node;

	return head;
}

int pop_front(list_node_t **head)
{
	if (head == NULL || *head == NULL)
	{
		return 0;
	}

	list_node_t *tmp = *head;
	int data = tmp->data;
	if ((*head)->next)
	{
		*head = (*head)->next;
		list_node_free(tmp);
	}
	else
	{
		list_node_free(tmp);
		*head = NULL;
		// (*head)->next = NULL;
	}

	return data;
}

int pop_back(list_node_t **head)
{
	if (head == NULL || *head == NULL)
	{
		return 0;
	}
	int data = 0;
	if (!(*head)->next)
	{
		data = (*head)->data;
		list_node_free(*head);
		*head = NULL;
	}
	else
	{
		list_node_t *tmp = *head;
		while (tmp->next->next)
		{
			tmp = tmp->next;
		}
		list_node_t *back = tmp->next;
		data = back->data;
		tmp->next = NULL;
		list_node_free(back);
	}

	return data;
}

void insert(list_node_t **head, list_node_t *elem, list_node_t *before)
{
	if (!*head && !before)
	{
		*head = elem;
	}
	else if (*head == before)
	{
		*head = push_front(*head, elem);
	}
	else
	{
		list_node_t *tmp = *head;
		while (tmp->next != before && tmp->next)
		{
			tmp = tmp->next;
		}
		if (tmp->next == before)
		{
			elem->next = before;
			tmp->next = elem;
		}
	}
}

void list_print(FILE *file, const list_node_t *head, printnode print)
{
	while (head)
	{
		print(file, head);
		head = head->next;
	}
}

void list_free(list_node_t *head)
{
	if (!head)
	{
		return;
	}
	list_node_t *tmp = head;
	// head = head->next;
	while (tmp)
	{
		head = head->next;
		list_node_free(tmp);
		tmp = head;
	}
}

errors_t list_copy(list_node_t *head, list_node_t **newhead)
{
	if (!newhead)
	{
		return ERR_ARGS;
	}

	if (!head)
	{
		*newhead = NULL;
		return EXIT_SUCCESSFUL;
	}

	list_node_t *tmp = NULL;
	while (head)
	{
		tmp = list_node_create(head->data/* , head->key */);
		if (!tmp)
		{
			list_free(*newhead);
			return ERR_MEM;
		}
		*newhead = push_end(*newhead, tmp);
		head = head->next;
	}

	return EXIT_SUCCESSFUL;
}

void append(list_node_t **head_a, list_node_t **head_b)
{
	if (!head_a || !head_b)
	{
		return;
	}

	if (*head_a == NULL)
	{
		*head_a = *head_b;
	}
	else
	{
		list_node_t *end = *head_a;
		while (end->next)
		{
			end = end->next;
		}
		end->next = *head_b;
	}
	*head_b = NULL;
}

void front_back_split(list_node_t *head, list_node_t **back)
{
	if (!back)
	{
		return;
	}

	if (!head)
	{
		*back = NULL;
		return;
	}
	list_node_t *fast = head;
	list_node_t *half = head;
	list_node_t *before = NULL;
	if (!head->next)
	{
		*back = NULL;
		return;
	}

	while (fast)
	{
		if (fast)
		{
			before = half;
			half = half->next;
			fast = fast->next;
		}
		if (fast)
		{
			fast = fast->next;
		}
	}

	if (before)
	{
		before->next = NULL;
	}
	*back = half;
}

/**
 * @brief Подфункция сортировки слиянием
 *
 * @details Вызывает саму себя до тех пор, пока не кончатся оба подсписка.
 *
 * @param[in,out] head_a Сортируемый левый подсписок
 * @param[in,out] head_b Сортируемый правый подсписок
 * @param[in] cmp Сравнитель
 * @return Указатель на новый узел
 */
list_node_t *sorted_merge(list_node_t **head_a, list_node_t **head_b, comparator cmp)
{
	if (!head_a || !head_b || !cmp)
	{
		return NULL;
	}
	list_node_t *res = NULL;
	list_node_t *tmp = NULL;

	if (!*head_a)
	{
		res = push_front(res, *head_b);
		*head_b = NULL;
		return res;
	}
	else if (!*head_b)
	{
		res = push_front(res, *head_a);
		*head_a = NULL;
		return res;
	}
	// if (*head_a == NULL)
	//     return *head_b;
	// else if (*head_b == NULL)
	//     return *head_a;

	int cmp_score = cmp(&(*head_a)->data, &(*head_b)->data);
	if (cmp_score <= 0)
	{
		res = push_front(res, *head_a);
		tmp = (*head_a)->next;
		(*head_a)->next = NULL;
		res->next = sorted_merge(&tmp, head_b, cmp);
	}
	else
	{
		res = push_front(res, *head_b);
		tmp = (*head_b)->next;
		(*head_b)->next = NULL;
		res->next = sorted_merge(head_a, &tmp, cmp);
	}
	// if (cmp_score <= 0)
	// {
	//     res = *head_a;
	//     res->next = sorted_merge(&(*head_a)->next, head_b, cmp);
	// }
	// else
	// {
	//     res = *head_b;
	//     res->next = sorted_merge(head_a, &(*head_b)->next, cmp);
	// }

	*head_a = NULL;
	*head_b = NULL;
	return res;
}

list_node_t *sort(list_node_t *head, comparator cmp)
{
	if (!head || !head->next)
	{
		return head;
	}
	list_node_t *back = NULL;
	front_back_split(head, &back);

	head = sort(head, cmp);
	back = sort(back, cmp);

	return sorted_merge(&head, &back, cmp);
}
