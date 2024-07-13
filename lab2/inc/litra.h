#ifndef LITRA_H__
#define LITRA_H__

#include <stdio.h>
#include "err.h"

#define AUTHOR_LEN 25
#define TITLE_LEN 25
#define PROD_LEN 25
#define BRANCH_LEN 25
#define TYPE_LEN 25


typedef enum translated
{
    native,
    translated
} translation_t;

typedef struct tech
{
    char branch[BRANCH_LEN + 1];
    translation_t translation;
    int year;
} tech_t;


typedef enum art
{
    roman,
    play,
    poetry
} art_t;

typedef struct artistic_t
{
    art_t type;
} artistic_t;


typedef enum baby
{
    tale,
    poem
} baby_t;

typedef struct child
{
    int min_year;
    baby_t type;
} child_t;

typedef enum form_t
{
    tech,    //"Technical"
    artistic,//"Artistic"
    childs   //"Child"
} form_t;

typedef struct book_t
{
    //Order at which struct is read
    //|----------------------------|
    //                             |
    //                             v
    union type_t                 //6
    {
        tech_t tech;
        artistic_t artistic;
        child_t childs;
    } type;
    char author[AUTHOR_LEN + 1]; //1
    char title[TITLE_LEN + 1];   //2
    char producer[PROD_LEN + 1]; //3
    form_t type_lit;             //5
    int number_pages;            //4
} book_t;

//На авторе ключик
typedef struct key_table
{
    char author[AUTHOR_LEN + 1];
    int index;
} key_table_t;

/**
 * @brief Функция чтения 1 книги из файла
 *
 * @param[in] file Указатель на файловую переменную
 * @param[out] book Указатель на считываемую структуру книга
 * @return Код ошибки
 */
int read_book_file(FILE *file, book_t *book);

/**
 * @brief Функция ввода 1 книги с помощью ввода руками
 *
 * @param[out] book Указатель на считываемую структуру
 * @return Код ошибки
 */
int read_book_by_hand(book_t *book);

/**
 * @brief Функция печати 1 книги
 *
 * @param[in] file Указатель на поток вывода
 * @param[in] book Указатель на печатуемую книгу
 */
void print_book(FILE *file, const book_t *book);

#endif

