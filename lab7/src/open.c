#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include "open.h"


open_hash_t open_init(void)
{
    open_hash_t table = malloc(sizeof(struct _open_hash_table));
    if (!table)
        return NULL;
    list_node_t **_table = calloc(START_SIZE, sizeof(list_node_t *));
    if (!_table)
    {
        free(table);
        return NULL;
    }
    table->table = _table;
    table->size = 0;
    table->capacity = START_SIZE;
    
    return table;
}

// void free_hash_table(open_hash_t table, size_t size)
void free_hash_table(open_hash_t table)
{
    for (size_t i = 0; i < table->capacity; i++)
    {
        list_free(table->table[i]);
    }
    free(table->table);
    free(table);
}

void clear_hash_table(open_hash_t table)
{
    for (size_t i = 0; i < table->capacity; i++)
    {
        list_free(table->table[i]);
        table->table[i] = NULL;
    }
}

size_t hash_of(int num)
{
    size_t x = num;

    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x);
    return x;
    // size_t x = num;
    // 
    // // x = ((x >> 1) * x + 3 * 7 * 101) * (x << 2) + (x << 2);
    // // x = 99983 / (x + 1);
    // x = (x * 19) << (x % 10);
    // x++;
    // return x;
}

size_t hash(int num, size_t capacity)
{
    return hash_of(num) % capacity; //возвращает индекс в массиве
}

bool need_to_reconstruct_add(size_t count, size_t capacity)
{
    return ((double) count >= (double) PERCENTAGE_ADD * capacity);
}

bool need_to_reconstruct_remove(size_t count, size_t capacity)
{
    return ((double) count <= (double) PERCENTAGE_REM * capacity) && count >= START_SIZE;
}

hash_error_t reconstruct(list_node_t **table_old, size_t capacity, list_node_t ***new_table, size_t *new_capacity, hash_mode_reconstruct_r mode)
{
    /*
     * calloc вместо malloc 
     * так как используется массив указателей и массив <<разряжен>>(элементы идут непоследовательно),
     * нужно определять иницилизировано ли место в массиве.
     * */

    //MODE = ADD --> MODE = 1
    if (mode)
        *new_capacity = (size_t) capacity * FACTOR;
    else
        *new_capacity = (size_t) capacity / FACTOR;
    *new_table = calloc(*new_capacity, sizeof(list_node_t *));
    if (!*new_table)
    {
        // node_free(new_node);
        return ERR_HASH_MEM;
    }
    list_node_t **curr = table_old;
    for (size_t i = 0; i < capacity; i++, curr++)
    {
        list_node_t *bucket = *curr;
        while (bucket)
        {
            list_node_t *copy = list_node_create(bucket->data/* , new_key */);
            if (!copy)
            {
                free(*new_table);
                *new_table = NULL;
                // free_hash_table(*new_table);
                return ERR_HASH_MEM;
            }
            (*new_table)[hash_of(bucket->data) % *new_capacity] = push_end((*new_table)[hash_of(bucket->data) % *new_capacity], copy);
            bucket = bucket->next;
        }
    }

    return OK_HASH;
}

hash_error_t open_find(const open_hash_t arr, int num)
{
    list_node_t *bucket = arr->table[hash(num, arr->capacity)];
    while (bucket)
    {
        if (bucket->data == num)
            return OK_HASH;
        bucket = bucket->next;
    }
    return NOT_FOUND_HASH;
}

hash_error_t open_find_zad(const open_hash_t arr, int num, int *count)
{
    list_node_t *bucket = arr->table[hash(num, arr->capacity)];
    while (bucket)
    {
        (*count)++;
        if (bucket->data == num)
            return OK_HASH;
        bucket = bucket->next;
    }
    return NOT_FOUND_HASH;
}

hash_error_t open_add(open_hash_t arr, int num)
{
    if (!open_find(arr, num))
        return IN_HASH;

    list_node_t *new_node = list_node_create(num);
    if (!new_node)
        return ERR_HASH_MEM;

    if (need_to_reconstruct_add(arr->size, arr->capacity))
    {
        list_node_t **new_table = NULL;
        size_t new_capacity = 0;
        hash_error_t err = reconstruct(arr->table, arr->capacity, &new_table, &new_capacity, ADD);
        if (!err)
        {
            // free_hash_table(arr);
            clear_hash_table(arr);
            free(arr->table);
            
            arr->table = new_table;
            arr->capacity = new_capacity;
        }
        else if (err == ERR_HASH_MEM)
        {
            list_node_free(new_node);
            return ERR_HASH_MEM;
        }
    }

    arr->table[hash(num, arr->capacity)] = push_end(arr->table[hash(num, arr->capacity)], new_node);
    arr->size++;

    return OK_HASH;
}

// assoc_array_error_t assoc_array_remove(assoc_array_t arr, const char *key)
hash_error_t open_remove(open_hash_t arr, int num)
{
    // if (assoc_array_find(arr, key, &a) == ASSOC_ARRAY_NOT_FOUND)
    //     return ASSOC_ARRAY_NOT_FOUND;

    if (open_find(arr, num) == NOT_FOUND_HASH)
        return ERR_HASH;

    size_t hash_ind = hash(num, arr->capacity);
    list_node_t *curr = arr->table[hash_ind], *prev = NULL;
    while (curr && curr->data != num)
    {
        prev = curr;
        curr = curr->next;
    }
    if (prev)
    {
        if (curr)
            prev->next = curr->next;
        else
            prev->next = NULL;
        list_node_free(curr);
    }
    else
    {
        if (curr)
            arr->table[hash_ind] = curr->next;
        else
            arr->table[hash_ind] = NULL;
        list_node_free(curr);
    }
    arr->size--;

    if (need_to_reconstruct_remove(arr->size, arr->capacity))
    {
        list_node_t **new_table = NULL;
        size_t new_capacity = 0;
        hash_error_t err = reconstruct(arr->table, arr->capacity, &new_table, &new_capacity, REMOVE);
        if (!err)
        {
            // free_hash_table(arr->hash_table, arr->capacity);
            clear_hash_table(arr);
            free(arr->table);
            arr->table = new_table;
            arr->capacity = new_capacity;
        }
        else if (err == ERR_HASH_MEM)
        {
            return ERR_HASH_MEM;
        }
    }
    return OK_HASH;
}

