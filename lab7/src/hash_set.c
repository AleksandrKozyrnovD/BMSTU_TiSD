#include "hash_set.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define START_SIZE 2
#define FACTOR 2

#define ERR_IO 2

void free_hash_set(hash_table_t table)
{
	free(table->table);
	free(table);
}

static size_t hash_of(int data)
{
    size_t x = data;
    
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x);
    return x;
    // x = ((x >> 1) * x + 3 * 7 * 101) * (x << 2) + (x << 2);
    // x = 999983 / (x + 1);
    // x = (x * 19) << (x % 10);
    // x++;
    // return x;
	// return data;
}

static size_t hash(hash_table_t arr, int key)
{
	return hash_of(key) % arr->capacity;
}

hash_table_t hash_table_create(void)
{
	hash_table_t hash_table = malloc(sizeof(struct hash_set));
	if (hash_table == NULL)
		return NULL;

	table_t table = calloc(START_SIZE, sizeof(value_t));
	if (table == NULL)
	{
		free(hash_table);
		return NULL;
	}
	hash_table->table = table;
	hash_table->size = 0;
	hash_table->capacity = START_SIZE;

	return hash_table;
}

static int reconstruct(hash_table_t arr)
{
	size_t new_capacity = (size_t) arr->capacity * FACTOR;
    table_t new_table;
hell:
    {
        new_table = calloc(new_capacity, sizeof(value_t));
        if (!new_table)
        {
            free(new_table);
            return 1;
        }

        // printf("===============\n");
        // value_t *curr = arr->table;

        for (size_t i = 0; i < arr->capacity; i++)
        {
            // printf("Curr->data = %d, init = %d\n", arr->table[i].data, arr->table[i].is_initialized);
            if (arr->table[i].is_initialized)
            {
                size_t index = hash_of(arr->table[i].data) % new_capacity;
                while (index < new_capacity && new_table[index].is_initialized)
                    index++;
                if (index == new_capacity)
                {
                    new_capacity *= FACTOR;
                    free(new_table);
                    goto hell;
                }
                new_table[index].data = arr->table[i].data;
                new_table[index].is_initialized = true;
            }
        }
    }
    // printf("===============\n");

	free(arr->table);
	arr->table = new_table;
	arr->capacity = new_capacity;
    
    // printf("Capacity after recon %zu\n", new_capacity);
	return 0;
}

int hash_set_insert(hash_table_t arr, int data)
{
	value_t *dummy = NULL;
	if (hash_table_find(arr, data, &dummy) == 0)
		return 1;

    if ((double) arr->size >= (double) 0.6 * arr->capacity)
    {
        if (reconstruct(arr))
            return 1;
    }

	size_t index = hash(arr, data);
    // printf("Index = %zu\n", index);
	if (!(arr->table[index].is_initialized))
	{
        // printf("Not init, so here\n");
		arr->table[index].data = data;
		arr->table[index].is_initialized = true;
		arr->size++;
		return 0;
	}
	else
	{
		for (size_t i = index; i < arr->capacity; i++)
		{
			if (arr->table[i].is_initialized == false)
			{
                // printf("i = %zu is not init\n", i);
				arr->table[i].data = data;
				arr->table[i].is_initialized = true;
				arr->size++;
				return 0;
			}
		}
        // printf("Reconstruct\n");
		// then it means that we need to reconstruct and try again
		if (reconstruct(arr))
            return 1;
		return hash_set_insert(arr, data);
	}
}

int hash_table_find(const hash_table_t arr, int key, value_t **out)
{
	for (size_t i = hash(arr, key); i < arr->capacity; i++)
	{
		value_t *curr = &arr->table[i];
		if (curr->is_initialized)
		{
			if (curr->data == key)
			{
				*out = curr;
				return 0;
			}
		}
	}

	return 1;
}

int hash_table_find_zad(const hash_table_t arr, int key, value_t **out, int *count)
{
	for (size_t i = hash_of(key) % arr->capacity; i < arr->capacity; i++)
	{
		// value_t *curr = &arr->table[i];
		if (arr->table[i].is_initialized)
		{
            (*count)++;
			if (arr->table[i].data == key)
			{
				*out = &arr->table[i];
				return 0;
			}
		}
	}

	return 1;
}

int hash_table_remove(hash_table_t arr, int key)
{
	value_t *value = NULL;
	if (hash_table_find(arr, key, &value) == 0)
	{
		value->data = 0;
		value->is_initialized = false;
		arr->size--;
		return 0;
	}

	return 1;
}

