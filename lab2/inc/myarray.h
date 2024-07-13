#ifndef MYARRAY_H__
#define MYARRAY_H__

#include "time.h"
#include <sys/time.h>
#include "err.h"
#include "litra.h"

typedef int (*comparator_t)(const void *, const void *);

/**
 * @brief Функция считывания всей таблицы книг из файла
 *
 * @param[in] file Поток ввода
 * @param[out] table Таблица книг
 * @param[out] sizetable Размер этой таблицы
 * @param[out] keys Ключи к этой таблице
 * @param[out] sizekey Размер ключей таблицы
 * @param[in,out] buffer_size Размер общего буффера
 * @return 
 */
int read_into_table(FILE *file, book_t **table, size_t *sizetable, key_table_t **keys, size_t *sizekey, size_t *buffer_size);

/**
 * @brief Печать таблицы в поток вывода
 *
 * @param[in] file Поток вывода
 * @param[in] table Таблица книг
 * @param[in] size Размер таблицы
 */
void print_table(FILE *file, const book_t *table, size_t size);

/**
 * @brief Печать только ключей и индексов их исходников в оригинальной таблице
 *
 * @param[in] file Поток вывода
 * @param[in] keys Ключи
 * @param[in] size Кол-во ключей
 */
void print_keys(FILE *file, const key_table_t *keys, size_t size);

/**
 * @brief Синхронизация ключей и самой таблицы
 *
 * @param[in,out] keys Ключи
 * @param[in,out] size_key Количество ключей
 * @param[in] table Таблица ключей
 * @param[in] size Размер таблицы
 */
void sync_key_with_table(key_table_t *keys, size_t *size_key, const book_t *table, size_t size);

/**
 * @brief Сравнитель книг
 *
 * @param[in] l Левая книга
 * @param[in] r Правая книга
 * @return Разность между ними (если левый больше, то больше нуля, иначе меньше нуля, как в математике)
 */
int comparator_book(const void *l, const void *r);

/**
 * @brief 
 *
 * @param[in] l Левый ключ
 * @param[in] r Правый ключ
 * @return Разность между ними (если левый больше, то больше нуля, иначе меньше нуля, как в математике)
 */
int comparator_key(const void *l, const void *r);

/**
 * @brief Сортировка пузырьком от тов. Ломовской
 *
 * @param[in,out] base Указатель на начало массива
 * @param[in] nmemb Кол-во элементов
 * @param[in] size Размер 1 элемента
 * @param[in] cmp Указатель на функцию сравнителя
 */
void buble_sort(void *base, size_t nmemb, size_t size, comparator_t cmp);

/**
 * @brief Удаление книги по имени автора
 *
 * @param[in,out] book Таблица книг
 * @param[in,out] size Размер этой таблицы
 * @return Код ошибки
 */
int delete_book_by_title(book_t *book, size_t *size);

/**
 * @brief Нахождение всех романов этого автора
 *
 * @param[in] file Поток вывода
 * @param[in] books Таблица книг
 * @param[in] size Размер этой таблицы
 * @param[in] author Имя автора, по которому происходит поиск
 * @return 
 */
int get_romans_author(FILE *file, const book_t *books, size_t size, char *author);

/**
 * @brief Выдает текущее время в мс
 */
unsigned long long microseconds_now(void);

/**
 * @brief Копирование массивов
 *
 * @param[in] src Указатель на источник
 * @param[in] nmemb Кол-во элементов
 * @param[in] size Размер 1 элемента
 * @param[out] dst Указатель на источник
 */
void copy_array(const void *src, size_t nmemb, size_t size, void *dst);

/**
 * @brief Взять время от сортировки таблицы указанной сортировкой
 *
 * @param[in] table Указатель на таблицы
 * @param[in] size Ее размер
 * @param[in] count Сколько раз проводим замеры
 * @param[in] sort Указатель на функцию сортировки
 * @param[in] orig Указатель на оригинальную таблицу книг
 * @return 
 */
double get_time_sort_table(book_t *table, size_t size, size_t count, void (*sort)(void *, size_t, size_t, comparator_t), const book_t *orig);

/**
 * @brief Взять время от сортировки ключей указанной сортировкой
 *
 * @param[in] keys Ключи
 * @param[in] size Размер ключей
 * @param[in] count Сколько раз проводим замеры
 * @param[in] sort Указатель на функцию сортировки
 * @param[in] orig Указатель на оригинальный массив ключей
 * @return 
 */
double get_time_sort_keys(key_table_t *keys, size_t size, size_t count, void (*sort)(void *, size_t, size_t, comparator_t), const key_table_t *orig);

#endif

