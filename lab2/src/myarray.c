#include "../inc/myarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Подсчитывает кол-во структур в файле
 *
 * @param[in] file Поток ввода
 * @param[out] size Размер таблицы
 * @return Код ошибки
 */
static int count_table(FILE *file, size_t *size)
{
    rewind(file);
    *size = 0;
    book_t book;

    int rc = read_book_file(file, &book);
    while (rc == EXIT_SUCCESSFUL)
    {
        (*size)++;
        rc = read_book_file(file, &book);
    }
    
    if (feof(file) && rc != EXIT_SUCCESSFUL)
        rc = EXIT_SUCCESSFUL;

    if (*size == 0)
        return EMPTY;

    rewind(file);

    return rc;
}

int read_into_table(FILE *file, book_t **table, size_t *sizetable, key_table_t **keys, size_t *sizekey, size_t *buffer_size)
{
    int rc;
    size_t size;
    book_t book;

    rc = count_table(file, &size);
    if (rc != EXIT_SUCCESSFUL)
        return rc;
    
    
    book_t *tmp_book = realloc(*table, size * sizeof(**table));
    if (tmp_book == NULL)
        return ERR_MEM;
    // *table = tmp_book;

    key_table_t *tmp_key = realloc(*keys, size * sizeof(**keys));
    if (tmp_key == NULL)
    {
        free(tmp_book);
        return ERR_MEM;
    }
    // *keys = tmp_key;
    // *table = calloc(size, sizeof(**table));
    // if (*table == NULL)
    //     return ERR_MEM;
    // *keys = calloc(size, sizeof(**keys));
    // if (*keys == NULL)
    // {
    //     free(*table);
    //     return ERR_MEM;
    // }
    
    key_table_t key;
    for (size_t i = 0; i < size; i++)
    {
        read_book_file(file, &book);
        // *table[i] = book;
        tmp_book[i] = book;
        strcpy(key.author, book.author);
        key.index = (int) i;
        tmp_key[i] = key;
    }
    if (*buffer_size < size)
        *buffer_size = size;
    
    *table = tmp_book;
    *keys = tmp_key;
    *sizetable = size;
    *sizekey = size;

    return EXIT_SUCCESSFUL;
}

void print_table(FILE *file, const book_t *table, size_t size)
{
    printf("\n%-5s| %-25s| %-25s| %-25s| %-6s| %-25s| %-25s| %-25s| %-25s|\n",
            "Ind", "Author", "Title", "Producer", "Pages", "Type", "Optional I", "Optional II", "Optional III");
    for (size_t i = 0; i < size; i++)
    {
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("%-5zu", i + 1);
        print_book(file, table + i);

    }
}

void print_keys(FILE *file, const key_table_t *keys, size_t size)
{
    printf("\n%-25s | %-25s\n", "Author", "Index Original");
    for (size_t i = 0; i < size; i++)
    {
        printf("---------------------------------------------------------------------------------------\n");
        fprintf(file, "%-25s | %-d\n", keys[i].author, keys[i].index);
    }
}

void sync_key_with_table(key_table_t *keys, size_t *size_key, const book_t *table, size_t size_table)
{
    //realloc zdes, poka chto udalenie
    if (*size_key >= size_table)
        *size_key = size_table;
    else
    {
        //realloc
        *size_key = size_table;
    }
    
    for (size_t i = 0; i < size_table; i++)
    {
        keys[i].index = i;
        strcpy(keys[i].author, table[i].author);
    }
}

int comparator_book(const void *l, const void *r)
{
    const book_t *right = r;
    const book_t *left = l;
    
    return strcmp(left->author, right->author);
}

int comparator_key(const void *l, const void *r)
{
    const key_table_t *left = l;
    const key_table_t *right = r;

    return strcmp(left->author, right->author);
}


// static void swap(void *l, void *r, size_t nmem)
// {
//     char tmp;
//     // (char *) l;
//     // (char *) r;
//
//     char *pl = l, *pr = r;
//     for (size_t i = 0; i < nmem; i++)
//     {
//         // tmp = *(pl + i);
//         // *(pl + i) = *(pr + i);
//         // *(pr + i) = tmp;
//         tmp = *pl;
//         *pl = *pr;
//         *pr = tmp;
//         pl++;
//         pr++;
//     }
// }
//
// void buble_sort(void *pdata, size_t nmem, size_t size, comparator_t cmp)
// {
//     if (!size)
//         return;
//
//     char *pbeg = pdata;
//     char *pend = pbeg + size * nmem;
//
//     for (char *ptop = pend; ptop > pbeg + nmem; ptop -= nmem)
//         for (char *pbot = pbeg + nmem; pbot < ptop; pbot += nmem)
//             if (cmp(pbot - nmem, pbot) > 0)
//                 swap(pbot - nmem, pbot, nmem);
// }

/**
 * @brief Функция бинарного поиска элемента в массиве
 *
 * @param[in] base Указатель на начало массива
 * @param[in] end Указатель на законец массива
 * @param[in] data Искомый элемент
 * @param[in] size Размер элементов
 * @param[in] cmp Указатель на функцию сравнитель
 * @return Указатель на область, где он должен лежать
 */
static void *binary_search(const void *base, const void *end, const void *data, size_t size, comparator_t cmp)
{
    const char *left = base;
    const char *right = end;
    if (left > right)
        return (void *) (left);

    const char *mid;
    int cmp_score;

    mid = (left + ((right - left) / size) / 2 * size);

    cmp_score = cmp(data, mid);

    if (cmp_score == 0)
        return (void *) mid;
    else if (cmp_score < 0)
        return binary_search(base, mid - size, data, size, cmp);
    else
        return binary_search(left + size, right, data, size, cmp);
}

void buble_sort(void *base, size_t nmemb, size_t size, comparator_t cmp)
{
    if (!size)
        return;

    // size_t index;
    
    // for (size_t i = 1; i < nmemb; ++i)
    // {
    //     void *curr = (char *) base + i * size;
    //     index = binary_search(base, curr, i, size, cmp);
    //     insert(base, i, size, index);
    // }

    char tmp[size]; //VLA
    char *pbeg = base;
    char *pend = pbeg + nmemb * size;
    char *index;
    
    for (char *pcur = pbeg + size; pcur < pend; pcur += size)
    {
        memcpy(tmp, pcur, size);
        index = binary_search(base, pcur - size, pcur, size, cmp);
        memmove(index + size, index, pcur - index);
        memcpy(index, tmp, size);
        // insert(index, pcur, tmp, size);
    }
}

/**
 * @brief Удаление вставками
 *
 * @param[in,out] books Указатель на таблицу книг
 * @param[in,out] size Размер таблицы
 * @param[in] index Какой индекс удалить
 */
static void books_delete(book_t *books, size_t *size, size_t index)
{
    (*size)--;
    for (size_t i = index; i < *size; i++)
        books[i] = books[i + 1];
}

int delete_book_by_title(book_t *books, size_t *size)
{
    char title[TITLE_LEN + 2];
    size_t index;
    int was = 0;

    printf("Введите название книги: ");
    if (!fgets(title, sizeof(title), stdin))
    {
        printf("Произошла ошибка при чтении названия книги!\n");
        return ERR_IO;
    }
    
    index = strlen(title);
    if (title[index - 1] == '\n')
        title[index - 1] = 0;
    index = 0;
    for (size_t i = 0; i < *size; i++)
    {
        if (strcmp(books[i].title, title))
            index++;
        else
        {
            was = 1;
            books_delete(books, size, index);
            break;
        }
    }
    
    return was == 0 ? NOT_FOUND : EXIT_SUCCESSFUL; //true if was delete
}

int get_romans_author(FILE *file, const book_t *books, size_t size, char *author)
{
    printf("\n%-5s| %-25s| %-25s| %-25s| %-6s| %-25s| %-25s| %-25s| %-25s|\n",
            "Ind", "Author", "Title", "Producer", "Pages", "Type", "Optional I", "Optional II", "Optional III");
    int was_roman = 0;

    for (size_t i = 0; i < size; i++)
    {
        if (books[i].type_lit == artistic && strcmp(books[i].author, author) == 0 && books[i].type.artistic.type == roman)
        {
            printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("%-5zu", i + 1);
            print_book(file, books + i);
            was_roman++;
        }
    }
    
    if (was_roman == 0)
    {
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("%-5s| %-25s| %-25s| %-25s| %-6s| %-25s| %-25s| %-25s| %-25s|\n", "/////", "/////////////////////////", "/////////////////////////", "/////////////////////////",
                "//////", "/////////////////////////", "/////////////////////////", "/////////////////////////", "/////////////////////////");
    }
    return was_roman;
}

unsigned long long microseconds_now(void)
{
    struct timeval value;

    if (gettimeofday(&value, NULL))
        return (unsigned long long) -1;
    
    return value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

void copy_array(const void *src, size_t nmemb, size_t size, void *dst)
{
    const char *source = src;
    char *dest = dst;
    
    for (const char *i = src; i < source + nmemb * size; i++, dest++)
        *dest = *i;
}

double get_time_sort_table(book_t *table, size_t size, size_t count, void (*sort)(void *, size_t, size_t, comparator_t), const book_t *orig)
{

    unsigned long long beg, end;
    double avg = 0.0;


    for (size_t i = 0; i < count; i++)
    {
        copy_array(orig, size, sizeof(book_t), table);
        beg = microseconds_now();
        sort(table, size, sizeof(*table), comparator_book);
        end = microseconds_now();
        avg += end - beg;

        // printf("Delta and Avg at i = %zu: %llu :: %.2lf\n", i, end - beg, avg / (i + 1));
    }
    return avg / count;
}

double get_time_sort_keys(key_table_t *keys, size_t size, size_t count, void (*sort)(void *, size_t, size_t, comparator_t), const key_table_t *orig)
{

    unsigned long long beg, end;
    double avg = 0.0;


    for (size_t i = 0; i < count; i++)
    {
        copy_array(orig, size, sizeof(key_table_t), keys);
        beg = microseconds_now();
        sort(keys, size, sizeof(*keys), comparator_key);
        end = microseconds_now();
        avg += end - beg;

        // printf("Delta and Avg at i = %zu: %llu :: %.2lf\n", i, end - beg, avg / (i + 1));
    }
    return avg / count;
}

