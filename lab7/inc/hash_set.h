#ifndef __HASH_SET_H__
#define __HASH_SET_H__

#include <stdbool.h>
#include <stddef.h>

typedef struct value
{
	int data;
	bool is_initialized;
} value_t;

typedef value_t *table_t;

struct hash_set
{
    /*
    Хеш-таблица.
    Является массивом данных, у которых есть флаг проинициализированности
    */
	table_t table;
	size_t size, capacity;
};

typedef struct hash_set *hash_table_t;

hash_table_t hash_table_create(void);

void free_hash_set(hash_table_t table);

// int reconstruct(hash_table_t arr);

int hash_set_insert(hash_table_t arr, int data);

int hash_table_find(const hash_table_t arr, int key, value_t **out);

int hash_table_find_zad(const hash_table_t arr, int key, value_t **out, int *count);

int hash_table_remove(hash_table_t arr, int key);

#endif // __HASH_SET_H__
