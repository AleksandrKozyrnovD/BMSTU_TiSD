#include <stdio.h>
#include <string.h>
#include "../inc/litra.h"

#define TECHNICAL "technical"
#define ARTISTIC "artistic"
#define CHILDS "childs"

/**
 * @brief Функция для считывания вариантных полей технических книг
 *
 * @param[in] file Поток ввожа
 * @param[out] book Считываемая книга
 * @return Код ошибки
 */
static int read_tech(FILE *file, book_t *book)
{
    char branch[BRANCH_LEN + 2], translation[15];
    int year;

    if (fscanf(file, "%24s %14s %d", branch, translation, &year) != 3)
    {
        // printf("Branch: %s, trans: %s, year: %d\n", branch, translation, year);
        return ERR_IO;
    }
    
    branch[BRANCH_LEN + 1] = 0;
    translation[14] = 0;

    if (strlen(branch) > BRANCH_LEN)
        return ERR_IO;
    if (strlen(translation) > 14)
        return ERR_IO;

    if (strcmp(translation, "native") == 0)
        book->type.tech.translation = native;
    else if (strcmp(translation, "translated") == 0)
        book->type.tech.translation = translated;
    else
        return ERR_IO;

    strcpy(book->type.tech.branch, branch);
    book->type.tech.year = year;


    return EXIT_SUCCESSFUL;
}

/**
 * @brief Функция для считывания вариантных полей хужожественной книги
 *
 * @param[in] file Поток ввожа
 * @param[out] book Считываемая книга
 * @return Код ошибки
 */
static int read_artistic(FILE *file, book_t *book)
{
    char type[10];
    if (fscanf(file, "%9s", type) != 1)
        return ERR_IO;

    if (strcmp(type, "roman") == 0)
        book->type.artistic.type = roman;
    else if (strcmp(type, "play") == 0)
        book->type.artistic.type = play;
    else if (strcmp(type, "poetry") == 0)
        book->type.artistic.type = poetry;
    else
        return ERR_IO;

    return EXIT_SUCCESSFUL;
}

/**
 * @brief Функция для считывания вариантных полей детских книг
 *
 * @param[in] file Поток ввожа
 * @param[out] book Считываемая книга
 * @return Код ошибки
 */
static int read_childs(FILE *file, book_t *book)
{
    int year;
    char type[15];

    if (fscanf(file, "%d %14s", &year, type) != 2)
        return ERR_IO;

    if (year < 0)
        return ERR_IO;

    // type[14] = 0;
    // if (strlen(type) > 12)
    //     return ERR_IO;

    if (strcmp(type, "tale") == 0)
        book->type.childs.type = tale;
    else if (strcmp(type, "poem") == 0)
        book->type.childs.type = poem;
    else
        return ERR_IO;

    book->type.childs.min_year = year;

    return EXIT_SUCCESSFUL;
}

int read_book_file(FILE *file, book_t *book)
{
    int rc;
    char author[AUTHOR_LEN + 2], title[TITLE_LEN + 2], producer[PROD_LEN + 2], type[25];
    int number_of_pages;
    form_t form;

    if (fscanf(file, "%24s %24s %24s %d %24s", author, title, producer, &number_of_pages, type) != 5)
        return ERR_IO;
//     printf("Test of invariant: \n");
    
    // author[AUTHOR_LEN + 1] = 0;
    // title[TITLE_LEN + 1] = 0;
    // producer[PROD_LEN + 1] = 0;
    // type[24] = 0;
    
    // printf("A: %zu\nT: %zu\nP:%zu\n", strlen(author),
    //         strlen(title), strlen(producer));
    if (strlen(author) > AUTHOR_LEN)
    {
        // printf("Author dlinniy\n");
        return ERR_IO;
    }
    if (strlen(title) > TITLE_LEN)
    {
        // printf("Title dlinniy\n");
        return ERR_IO;
    }
    if (strlen(producer) > PROD_LEN)
    {
        // printf("Prod dlinniy\n");
        return ERR_IO;
    }
    if (number_of_pages <= 0)
    {
        return ERR_IO;
    }

    // printf("Test of invariant: \n");
    // printf("Author: %s, Title: %s, produced: %s, type: %s, pages: %d\n", author, title, producer, type, number_of_pages);
    
    if (strcmp(type, TECHNICAL) == 0)
        form = tech;
    else if (strcmp(type, ARTISTIC) == 0)
        form = artistic;
    else if (strcmp(type, CHILDS) == 0)
        form = childs;
    else
        return ERR_IO;

    switch (form)
    {
        case tech:
            rc = read_tech(file, book);
            break;
        case artistic:
            rc = read_artistic(file, book);
            break;
        case childs:
            rc = read_childs(file, book);
            break;
    }

    book->number_pages = number_of_pages;
    strcpy(book->author, author);
    strcpy(book->title, title);
    strcpy(book->producer, producer);
    book->type_lit = form;
    
    return rc;
}

void print_book(FILE *file, const book_t *book)
{
    // fprintf(file, ">>----------------------------------------------------<<\n");
    // fprintf(file, "Author: %s, Title: %s\nProducer: %s, Pages ->%d<-\n",
    //         book->author, book->title,
    //         book->producer, book->number_pages);
    
    fprintf(file, "| %-25s| %-25s| %-25s| %-6d|", book->author, book->title, book->producer, book->number_pages);
    switch (book->type_lit)
    {
        case tech:
            // fprintf(file, "Type: Technical, Branch: %s, year %d, ",
            // book->type.tech.branch, book->type.tech.year);
            fprintf(file, " %-25s| %-25s| %-25d|", "Technical", book->type.tech.branch, book->type.tech.year);
            switch (book->type.tech.translation)
            {
                case native:
                    fprintf(file, " %-25s|\n", "native");
                    break;
                case translated:
                    // fprintf(file, "Translation: translated\n");
                    fprintf(file, " %-25s|\n", "translated");
                    break;
                default:
                    // fprintf(file, "Translation: None\n");
                    fprintf(file, " %-25s|\n", "None");
                    break;
            }
            break;
        case artistic:
            // fprintf(file, "Type: artistic, ");
            fprintf(file, " %-25s|", "artistic");
            switch (book->type.artistic.type)
            {
                case roman:
                    // fprintf(file, "Roman\n");
                    fprintf(file, " %-25s|", "Roman");
                    break;
                case play:
                    // fprintf(file, "Play\n");
                    fprintf(file, " %-25s|", "Play");
                    break;
                case poetry:
                    // fprintf(file, "Poetry\n");
                    fprintf(file, " %-25s|", "Poetry");
                    break;
                default:
                    // fprintf(file, "None\n");
                    fprintf(file, " %-25s|", "None");
                    break;
            }
            fprintf(file, " %25s|", "/////////////////////////");
            fprintf(file, " %25s|\n", "/////////////////////////");
            break;
        case childs:
            // fprintf(file, "Type: for childs %d+, ", book->type.childs.min_year);
            fprintf(file, " %-25s| %-25d|", "Children's lit-ra", book->type.childs.min_year);
            switch (book->type.childs.type)
            {
                case tale:
                    fprintf(file, " %-25s|", "Tale");
                    // fprintf(file, "Tale\n");
                    break;
                case poem:
                    fprintf(file, " %-25s|", "Poem");
                    // fprintf(file, "Poem\n");
                    break;
                default:
                    fprintf(file, " %-25s|", "None");
                    // fprintf(file, "None\n");
                    break;
            }
            // fprintf(file, "%25s|", "/////////////////////////");
            fprintf(file, " %25s|\n", "/////////////////////////");
            break;
    }
    // fprintf(file, ">>----------------------------------------------------<<\n");
}

/**
 * @brief Функция для удаления символа /n их конца строк
 *
 * @param buffer Строка
 * @param len Размер этой строки
 */
static void remove_n(char *buffer, size_t *len)
{
    if (buffer[*len - 1] == '\n')
        buffer[--(*len)] = 0;
}

int read_book_by_hand(book_t *book)
{
    char author[AUTHOR_LEN + 2], title[TITLE_LEN + 2], producer[PROD_LEN + 2], tmp[2];
    int number_of_pages;
    int type_of_choice; //Для enum'ов
    form_t form;
    size_t len;

    printf("Введите имя автора (не больше %d символов):\n--> ", AUTHOR_LEN);
    if (!fgets(author, sizeof(author), stdin))
    {
        printf("Случилась ошибка при вводе имени автора!\n");
        return ERR_IO;
    }

    len = strlen(author);
    remove_n(author, &len);

    if (!len)
    {
        printf("Имя автора пустое\n");
        return ERR_IO;
    }
    else if (len > AUTHOR_LEN)
    {
        printf("Имя автора слишком длинное! Нельзя вводить больше %d символов\n", AUTHOR_LEN);
        return ERR_IO;
    }

    printf("Введите название книги (не больше %d символов):\n--> ", TITLE_LEN);
    if (!fgets(title, sizeof(title), stdin))
    {
        printf("Случилась ошибка при вводе имени книги!\n");
        return ERR_IO;
    }

    len = strlen(title);
    remove_n(title, &len);

    if (!len)
    {
        printf("Имя книги пустое\n");
        return ERR_IO;
    }
    else if (len > TITLE_LEN)
    {
        printf("Имя книги слишком длинное! Нельзя вводить больше %d символов\n", TITLE_LEN);
        return ERR_IO;
    }

    printf("Введите название издателя (не больше %d символов):\n--> ", PROD_LEN);
    if (!fgets(producer, sizeof(producer), stdin))
    {
        printf("Случилась ошибка при вводе издателя!\n");
        return ERR_IO;
    }

    len = strlen(producer);
    remove_n(producer, &len);

    if (!len)
    {
        printf("Имя издателя пустое\n");
        return ERR_IO;
    }
    else if (len > TITLE_LEN)
    {
        printf("Имя издателя слишком длинное! Нельзя вводить больше %d символов\n", PROD_LEN);
        return ERR_IO;
    }
    
    printf("Введите количество страниц этой книги в виде числа:\n--> ");
    if (scanf("%d", &number_of_pages) != 1)
    {
        fgets(tmp, sizeof(tmp), stdin);
        printf("Кол-во страниц введено неверно!\n");
        return ERR_IO;
    }
    fgets(tmp, sizeof(tmp), stdin);
    
    if (number_of_pages <= 0)
    {
        printf("Кол-во страниц введено неверно! Вы ввели неотрицательное число\n");
        return ERR_IO;
    }

    
    printf("Введите тип книги в виде цифры:\n 1) Техническая\n 2) Художественная\n 3) Книги для детей\n--> ");
    if (scanf("%d", &type_of_choice) != 1)
    {
        fgets(tmp, sizeof(tmp), stdin);
        printf("Неверный ввод типа книги! Введите число 1 или 2 или 3\n");
        return ERR_IO;
    }
    fgets(tmp, sizeof(tmp), stdin);
    form = type_of_choice - 1;
    // printf("Form: %d, tech: %d, art: %d, child: %d\n", form, tech, artistic, childs);
    
    strcpy(book->author, author);
    strcpy(book->title, title);
    strcpy(book->producer, producer);
    book->number_pages = number_of_pages;

    //Bариантные поля

    //Техническая
    char branch[BRANCH_LEN + 2];
    translation_t translation;
    int year;

    //Художественная
    art_t type;

    //Для дiтей
    baby_t baby;
    //int year;
    
    //Конец вариантных полей
    
    switch (form)
    {
        case tech:
            printf("Введите отрасль, про которую написана эта книга (не больше %d символов):\n--> ", BRANCH_LEN);
            if (!fgets(branch, sizeof(branch), stdin))
            {
                printf("Случилась ошибка при чтении отрасли!\n");
                return ERR_IO;
            }
            len = strlen(branch);
            remove_n(branch, &len);
            
            if (!len)
            {
                printf("Вы ввели пустую отрасль!\n");
                return ERR_IO;
            }
            else if (len > BRANCH_LEN)
            {
                printf("Вы ввели слишком длинное название отрасли! (Не вводите больше %d символов)\n", BRANCH_LEN);
                return ERR_IO;
            }
            
            printf("Введите тип перевода: \n 1) Отечественная \n 2) Переводная\n--> ");
            if (scanf("%d", &type_of_choice) != 1)
            {
                fgets(tmp, sizeof(tmp), stdin);
                printf("Можно вводить только 1 или 2!\n");
                return ERR_IO;
            }
            fgets(tmp, sizeof(tmp), stdin);
            translation = type_of_choice - 1;
            if (translation != native && translation != translated)
            {
                printf("Можно вводить только 1 или 2!\n");
                return ERR_IO;
            }

            printf("Введите год создания в виде числа (отрицательный = до н.э):\n--> ");
            if (scanf("%d", &year) != 1)
            {
                fgets(tmp, sizeof(tmp), stdin);
                printf("Введить год только в виде числа\n");
                return ERR_IO;
            }
            fgets(tmp, sizeof(tmp), stdin);
            book->type_lit = form;
            book->type.tech.translation = translation;
            strcpy(book->type.tech.branch, branch);
            book->type.tech.year = year;
            break;
        case artistic:
            printf("Введите тип художества:\n 1) Роман\n 2) Пиеса\n 3) Поэзия\n--> ");
            if (scanf("%d", &type_of_choice) != 1)
            {
                fgets(tmp, sizeof(tmp), stdin);
                printf("Можно ввести только 1 или 2 или 3\n");
                return ERR_IO;
            }
            fgets(tmp, sizeof(tmp), stdin);
            type = type_of_choice - 1;
            if (type != roman && type != play && type != poetry)
            {
                printf("Можно ввести только 1 или 2 или 3\n");
                return ERR_IO;
            }
            book->type_lit = form;
            book->type.artistic.type = type;
            break;
        case childs:
            printf("Введите минимальный возраст к книге в виде числа:\n--> ");
            if (scanf("%d", &year) != 1)
            {
                fgets(tmp, sizeof(tmp), stdin);
                printf("Введить можно только целое неотрицательное число\n");
                return ERR_IO;
            }
            fgets(tmp, sizeof(tmp), stdin);
            if (year < 0)
            {
                printf("Введите целое неотрицательное число\n");
                return ERR_IO;
            }

            printf("Введите тип книги для детей:\n 1) Сказка\n 2) Стихи\n--> ");
            if (scanf("%d", &type_of_choice) != 1)
            {
                printf("Введить можно только 1 или 2\n");
                return ERR_IO;
            }
            baby = type_of_choice - 1;
            if (baby != tale && baby != poem)
            {
                printf("Введить можно только 1 или 2\n");
                return ERR_IO;
            }
            book->type_lit = form;
            book->type.childs.min_year = year;
            book->type.childs.type = baby;

            break;
        default:
            printf("Неверный ввод типа книги! Введить можно только число 1 или 2 или 3\n");
            return ERR_IO;
    }

    return EXIT_SUCCESSFUL;
}

