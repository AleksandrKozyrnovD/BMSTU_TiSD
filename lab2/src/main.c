#include "../inc/litra.h"
#include "../inc/menu.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    int rc;
    book_t *books, *test_book;
    key_table_t *keys, *test_keys;
    size_t size_books = 0, size_keys = 0;
    size_t buffer_size = 1;
    char filename[256], tmp[2];
    FILE *file = NULL;
    book_t book;
    double avg_table_buble, avg_table_qsort;
    double avg_keys_buble, avg_keys_qsort;
    int action = -1, count;

    books = calloc(1, sizeof(*books));
    if (!books)
    {
        printf("Не хватает памяти\n");
        return ERR_MEM;
    }
    keys = calloc(1, sizeof(*keys));
    if (!keys)
    {
        printf("Не хватает памяти\n");
        free(books);
        return ERR_MEM;
    }

    while (action != exit_menu)
    {
        help_print();
        if (scanf("%d", &action) != 1)
        {
            fgets(tmp, sizeof(tmp), stdin);
            printf("Введите пункт меню\n");
            continue;
        }
        fgets(tmp, sizeof(tmp), stdin);

        switch (action)
        {
            case exit_menu:
                break;
            case load:
                printf("Введите имя файла: ");
                if (!fgets(filename, sizeof(filename), stdin))
                {
                    printf("Произошла ошибка при чтении имени файла!\n");
                    break;
                }
                size_t len = strlen(filename);
                if (filename[len - 1] == '\n')
                    filename[len - 1] = 0;
                file = fopen(filename, "r");
                if (!file)
                {
                    printf("Файла не существует или не может быть открыт\n");
                    break;
                }
                rc = read_into_table(file, &books, &size_books, &keys, &size_keys, &buffer_size);
                if (rc != EXIT_SUCCESSFUL)
                {
                    if (file != NULL)
                        fclose(file);
                    printf("Файл некорректно заполнен или пуст\n");
                    break;
                }
                if (file != NULL)
                    fclose(file);
                printf("Данные загружены в программу\n");
                break;
            case show_not_sorted:
                if (size_books == 0)
                    printf("Ничего нет\n");
                else
                    print_table(stdout, books, size_books);
                break;
            case add_end:
                // printf("При вводе нескольких полей, разделенных пробелами, в одной строчке, будет считаться, что вы вводите сразу несколько полей!\n");
                rc = read_book_by_hand(&book);
                if (rc != EXIT_SUCCESSFUL)
                {
                    // printf("At add_end rc = %d\n", rc);
                    break;
                }
                size_books++;
                size_keys++;
                if (size_books > buffer_size)
                {
                    buffer_size = size_books * 2;
                    book_t *tmp_book = realloc(books, buffer_size * sizeof(*books));
                    if (tmp_book == NULL)
                    {
                        printf("Произошла непредвиденная ошибка! :(\n");
                        break;
                    }
                    books = tmp_book;
                    key_table_t *tmp_key = realloc(keys, buffer_size * sizeof(*keys));
                    if (tmp_key == NULL)
                    {
                        printf("Произошла непредвиденная ситуация! Не хватает памяти!\n");
                        free(books);
                        free(keys);
                        return ERR_MEM;
                    }
                    keys = tmp_key;
                }
                books[size_books - 1] = book;
                sync_key_with_table(keys, &size_keys, books, size_books);
                printf("Запись добавлена в конец таблицы\n");
                break;
            case delete_by_title_first:
                rc = delete_book_by_title(books, &size_books);
                if (rc == NOT_FOUND)
                {
                    printf("Ничего не удалено по вашему запросу\n");
                    break;
                }
                sync_key_with_table(keys, &size_keys, books, size_books);
                break;
            case print_by_keys:
                if (size_keys == 0)
                    printf("Пусто\n");
                // qsort(keys, size_keys, sizeof(*keys), comparator_key);
                print_keys(stdout, keys, size_keys);
                break;
            case print_table_by_table:
                if (size_books == 0)
                    printf("Пусто\n");
                qsort(books, size_books, sizeof(*books), comparator_book);
                sync_key_with_table(keys, &size_keys, books, size_books);
                print_table(stdout, books, size_books);
                break;
            case print_table_by_sorted_keys:
                qsort(keys, size_keys, sizeof(*keys), comparator_key);
                if (size_books != size_keys)
                {
                    printf("At print table by keys sizes not equal\n");
                }
                if (size_books == 0)
                    printf("Пусто\n");

                printf("\n%-5s| %-25s| %-25s| %-25s| %-6s| %-25s| %-25s| %-25s| %-25s|\n",
                        "Ind", "Author", "Title", "Producer", "Pages", "Type", "Optional I", "Optional II", "Optional III");
                for (size_t i = 0; i < size_books; i++)
                {
                    printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
                    printf("%-5zu", i + 1);
                    print_book(stdout, &books[keys[i].index]);

                }
                break;
            case print_roman_author:
                printf("Введите имя автора, чьи романы вы будете искать: ");
                char author[AUTHOR_LEN + 2];
                if (!fgets(author, sizeof(author), stdin))
                {
                    printf("Случилась ошибка при чтении имени автора!\n");
                    break;
                }

                if (author[strlen(author) - 1] == '\n')
                    author[strlen(author) - 1] = 0;
                rc = get_romans_author(stdout, books, size_books, author);
                if (rc == 0) //count == 0
                    printf("\nНичего не найдено!\n");
                break;
            case get_time_sort:
                if (size_books == 0)
                {
                    printf("Нечего замерять!\n");
                    break;
                }
                test_book = malloc(size_books * sizeof(book_t));
                if (!test_book)
                {
                    printf("Не хватает памяти!\n");
                    break;
                }
                test_keys = malloc(size_keys * sizeof(key_table_t));
                if (!test_keys)
                {
                    free(test_book);
                    printf("Не хватает памяти!\n");
                    break;
                }
                printf("\nАлгоритм замеров:\n 1) Берется текущая таблица и ключи к ней\n 2) Они копируются в новые массивы\n 3) Они сортируются n раз, после снова копируются из оригиналов. n - введенное пользователем число\n 4) Результаты усредняются и анализируются\n");
                printf("\n\nВведите количество замеров. \nПрошу заметить, что при больших кол-вах замеров программа оптимизирует обращение к часто адресуемым ячейкам (кеширует), отчего результат может показаться неправдоподобным\nСвязано это с тем, что, сортируя одинаковую таблицу n раз, программа начинает сохранять часто адресуемые и похожие данные\nПроизводится сравнение Qsort и вставки с двоичным поиском\n--> ");
                if (scanf("%d", &count) != 1)
                {
                    fgets(tmp, sizeof(tmp), stdin);
                    free(test_keys);
                    free(test_book);
                    printf("Неверно ввели количество замеров!\n");
                    break;
                }
                if (count <= 0)
                {
                    free(test_keys);
                    free(test_book);
                    printf("Неверно ввели количество замеров!\n");
                    break;
                }
                fgets(tmp, sizeof(tmp), stdout);
                
                printf("\nОжидайте результаты.\n");
                printf(" -QSORT таблица\n");
                avg_table_qsort = get_time_sort_table(test_book, size_books, count, qsort, books);
                printf(" -InsertSort таблица\n");
                avg_table_buble = get_time_sort_table(test_book, size_books, count, buble_sort, books);
                
                printf(" -QSORT ключи\n");
                avg_keys_qsort = get_time_sort_keys(test_keys, size_keys, count, qsort, keys);
                printf(" -InsertSort ключи\n");
                avg_keys_buble = get_time_sort_keys(test_keys, size_keys, count, buble_sort, keys);
                
                printf("\nСреднее время сортировок:\n");
                printf(" -Сортировка дв. вставками ключей: %.2lfмс\n", avg_keys_buble);
                printf(" -Сортировка дв. вставками таблицы: %.2lfмс\n", avg_table_buble);
                printf(" -Qsort сортировка ключей: %.2lfмс\n", avg_keys_qsort);
                printf(" -Qsort сортировка таблицы: %.2lfмс\n", avg_table_qsort);

                printf("\nАнализируем...\n");
                printf(" -Qsort на %.2lf%% более эффективен, чем сортировка вставками для Таблиц\n", (avg_table_buble - avg_table_qsort) / avg_table_buble * 100);
                printf(" -Qsort на %.2lf%% более эффективен, чем сортировка вставками для Ключей\n", (avg_keys_buble - avg_keys_qsort) / avg_keys_buble * 100);
                
                printf("\nИсследование одинаковых сортировок, то есть исследование эффективности использования ключей\n");
                printf(" -Qsort sort: ключи на %.2lf%% более эффективны, чем сортировка таблицы\n", (avg_table_qsort - avg_keys_qsort) / avg_table_qsort * 100);
                printf(" -Insert sort: Ключи на  %.2lf%% более эффективны, чем сортировка таблицы\n", (avg_table_buble - avg_keys_buble) / avg_table_buble * 100);

                printf("\nРазмеры: \n");
                printf(" -Таблицы: %zu байт\n", sizeof(*books) * size_books);
                printf(" -Таблицы + ключи: %zu байт\n", sizeof(*books) * size_books + sizeof(*keys) * size_keys);
                printf(" -Ключи: %zu байт\n", sizeof(*keys) * size_keys);
                printf("Таблица с ключами занимает на %.2lf%% больше места, чем сама таблица\n", ((double) (sizeof(*books) * size_books + sizeof(*keys) * size_keys)) / ((double) (sizeof(*books) * size_books)) * 100 - 100);

                free(test_keys);
                free(test_book);
                break;
            default:
                printf("Выберите пункты меню!\n");
                break;
        }
    }

    free(books);
    free(keys);

    return 0;
}
