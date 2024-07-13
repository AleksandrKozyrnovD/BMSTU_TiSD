#ifndef __OPEN_H__
#define __OPEN_H__

#include <stddef.h>
#include <stdbool.h>
#include "list.h"
#include "node.h"

#define START_SIZE 2
#define FACTOR 2
#define PERCENTAGE_ADD 0.8
#define PERCENTAGE_REM 0.3

struct _open_hash_table
{
    /*
    Хеш таблица
    Является массивом списков
    */
    list_node_t **table;
    size_t size, capacity;
};

typedef struct _open_hash_table *open_hash_t;


typedef enum hash_error
{
    OK_HASH, ERR_HASH_MEM, ERR_HASH, NOT_FOUND_HASH, IN_HASH
} hash_error_t;

typedef enum hash_mem_manage
{
    REMOVE = 0, ADD
} hash_mode_reconstruct_r;


open_hash_t open_init(void);

/**
 * @brief Освобождение памяти из-под хеш-таблицы
 *
 * @param[in] table Хеш таблица
 * @param[in] size Размер хеш-таблицы
 */
void free_hash_table(open_hash_t table);

/**
 * @brief Очищение элементов хеш-таблицы
 *
 * @param[in] table Хеш таблица
 * @param[in] size Размер хеш-таблицы
 */
void clear_hash_table(open_hash_t table);

/**
 * @brief Хеш строки
 *
 * @param[in] key Строка-ключ
 * @return Хеш-значение
 */
size_t hash_of(int num);

/**
 * @brief Хеш-индекс в хеш-таблице
 *
 * @param[in] key Строка-ключ
 * @param[in] capacity Размер хеш-таблицы
 * @return Индекс в хеш-таблице
 */
size_t hash(int num, size_t capacity);

hash_error_t open_find(const open_hash_t table, int num);

hash_error_t open_find_zad(const open_hash_t table, int num, int *count);
/**
 * @brief Нужно ли пересобирать хеш-таблицу при добавлении
 *
 * @param[in] count Ее заполненный размер
 * @param[in] capacity Выделенный размер
 * @return Правда или ложь
 */
bool need_to_reconstruct_add(size_t count, size_t capacity);

/**
 * @brief Нужно ли пересобирать хеш-таблицу при удалении
 *
 * @param[in] count Ее заполненный размер
 * @param[in] capacity Выделенный размер
 * @return Правда или ложь
 */
bool need_to_reconstruct_remove(size_t count, size_t capacity);

/**
 * @brief Перестройка текущей хеш-таблицы
 *
 * @param[in] table_old Старая хеш-таблица
 * @param[in] capacity_old Старый размер хеш-таблицы
 * @param[out] new_table Новая хеш-таблица
 * @param[out] capacity_new Новый размер хеш-таблицы
 * @param[in] new_node Узел, который будет вставлен
 * @return Код ошибки
 */
hash_error_t reconstruct(list_node_t **table_old, size_t capacity_old, list_node_t ***new_table, size_t *capacity_new, hash_mode_reconstruct_r mode);

hash_error_t open_add(open_hash_t table, int num);

hash_error_t open_remove(open_hash_t table, int num);

#endif

