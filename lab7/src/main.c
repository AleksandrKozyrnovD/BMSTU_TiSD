#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include "avl_node.h"
#include "hash_set.h"
#include "tree.h"
#include "avl_tree.h"
#include "stack.h"
#include "open.h"

#define COUNT_TEST 10000

void avl_node_clear(avl_node_t *root, void *param)
{
    (void) param;
    free(root);
}

void append_to_arr(tree_node_t *root, void *param)
{
    // if (root->option == ' ')
    // {
    //     // root->value = -101010101;
    //     return;
    // }
    int **ptr = param;
    if (root->option != ' ')
    {
        **ptr = root->value;
        (*ptr)++;
    }
}

unsigned long long microseconds_now(void)
{
    struct timeval value;

    if (gettimeofday(&value, NULL))
        return (unsigned long long) -1;
    
    return value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

static void help(void)
{
    printf("\n---------------------------\n");
    printf(" 1) Ввести выражение\n");
    printf(" 2) Посчитать с помощью дерева\n");
    printf(" 3) Добавить в дерево узел\n");
    printf(" 4) Удалить из дерева узел\n");
    printf(" 5) Обойти введенное дерево префиксно\n");
    printf(" 6) Обойти введенное дерево инфиксно\n");
    printf(" 7) Обойти введенное дерево постфиксно\n");
    printf(" 8) Выгрузить дерево в файл\n");
    printf(" 9) Очистить дерево\n");
    printf(" 10) Добавить элемент в хеш-таблицу с открытой адресацией\n");
    printf(" 11) Удалить элемент из хеш-таблицы с открытой адресацией\n");
    printf(" 12) Найти элемент в хеш-таблице с открытой адресацией\n");
    printf(" 13) Добавить элемент в хеш-таблицу с закрытой адресацией\n");
    printf(" 14) Удалить элемент из хеш-таблицы с закрытой адресацией\n");
    printf(" 15) Найти элемент в хеш-таблице с закрытой адресацией\n");
    printf(" 16) Посчитать эффективность\n");
    printf(" 17) Вывести хеш-таблицу с открытой адресацией\n");
    printf(" 18) Вывести хеш-таблицу с закрытой адресацией\n");
    printf(" 19) Закончить работу программы\n");
    printf("\n---------------------------\n");
}


int main(void)
{
    hash_table_t open = hash_table_create();
    if (!open)
    {
        printf("Не хватает памяти!\n");
        return ERR_MEM;
    }
    open_hash_t closed = open_init();
    if (!closed)
    {
        printf("Не хватает памяти!\n");
        return ERR_MEM;
    }

    hash_table_t open_zad = NULL;
    open_hash_t closed_zad = NULL;

    value_t *dummy = NULL;
    int arr[9];
    int arr_values[9];
    avl_node_t *root_avl = NULL, *root_avl_zad = NULL;
    (void) root_avl;
    tree_node_t *root = NULL;
    // tree_node_t *tmp_node = NULL;
    int oper = -1, input = 0, count = 0/* , randint = 0 */;
    int was_inputed = 0;
    unsigned long long begin, end;
    FILE *file = NULL;
    char tmp[256];
    srand(time(NULL));

    while (oper != 19)
    {
        help();
        printf(" Выберите команду: ");
        if (scanf("%d", &oper) != 1)
        {
            // fgets(tmp, sizeof(tmp), stdin);
            oper = -1;
            printf("Неверно введена команда\n");
            // break;
        }
        fgets(tmp, sizeof(tmp), stdin);

        switch (oper)
        {
            case 19:
                postfix(root, node_clear);
                postorder_param(root_avl, avl_node_clear, NULL);
                free_hash_table(closed);
                free_hash_set(open);
                break;
            case 1:
                printf("Введите %d целочисленных значений для A, B, C ... I: ", 9);
                int i;
                for (i = 0; i < 9; i++)
                {
                    if (scanf("%d", &arr[i]) != 1)
                    {
                        fgets(tmp, sizeof(tmp), stdin);
                        printf("Ошибка! Требуется целое число.\n");
                        break;
                    }
                    // fgets(tmp, sizeof(tmp), stdin);
                }
                fgets(tmp, sizeof(tmp), stdin);
                if (i == 9)
                {
                    printf("\n\nПолученное выражение:\n");
                    printf("%d + (%d * (%d + (%d * (%d + %d) - (%d - %d)) + %d))\n\n",
                    arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7], arr[8]);
                    was_inputed = 1;
                }
                break;
            case 2:
                if (!was_inputed)
                {
                    printf("Вы не ввели выражение\n");
                    break;
                }
                tree_node_t *root_zad = create_tree(arr);
                if (!root_zad)
                {
                    printf("Ошибка при выделении памяти\n");
                    break;
                }
                postfix_zad(root_zad, 0);
                printf("Результат: %d\n", root_zad->value);
                infix(root_zad, node_print_int);
                int *temp_ptr = &arr_values[0];
                infix_param(root_zad, append_to_arr, &temp_ptr);
    
                for (size_t i = 0; i < 8; i++)
                    root_avl_zad = tree_insert(root_avl_zad, arr_values[i]);
                
                open_zad = hash_table_create();
                closed_zad = open_init();

                for (size_t i = 0; i < 8; i++)
                {
                    hash_set_insert(open_zad, arr_values[i]);
                    open_add(closed_zad, arr_values[i]);
                }
                
                printf("\nДля открытой адресации:\n");
                for (size_t i = 0; i < open_zad->capacity; i++)
                {
                    printf("%zu) ", i + 1);
                    if (open_zad->table[i].is_initialized)
                    {
                        printf("Число: %d, хеш: %zu", open_zad->table[i].data, hash_of(open_zad->table[i].data) % open_zad->capacity);
                    }
                    printf("\n");
                }

                printf("Для закрытой адресации:\n");
                for (size_t i = 0; i < closed_zad->capacity; i++)
                {
                    printf("%zu) ", i + 1);
                    const list_node_t *bucket = closed_zad->table[i];
                    while (bucket)
                    {
                        printf("Число: %d, хеш: %zu -> ", bucket->data, hash_of(bucket->data) % closed_zad->capacity);
                        bucket = bucket->next;
                    }
                    printf("NULL\n");
                }
                file = fopen("avl_tree.dot", "w");
                avl_export_to_dot(file, "AVL_tree", root_avl_zad);
                fclose(file);
                file = NULL;
                free_hash_set(open_zad);
                free_hash_table(closed_zad);
                open_zad = NULL;
                closed_zad = NULL;
                postorder_param(root_avl_zad, avl_node_clear, NULL);
                postfix(root_zad, node_clear);
                break;
            case 3:
                printf("Введите число: ");
                if (scanf("%d", &input) != 1)
                {
                    fgets(tmp, sizeof(tmp), stdin);
                    printf("Неверно введено число\n");
                    break;
                }
                fgets(tmp, sizeof(tmp), stdin);
                if (tree_search(root_avl, input))
                {
                    printf("Такой уже есть\n");
                    break;
                }
                root_avl = tree_insert(root_avl, input);
                // if (find_in_tree(root, input))
                // {
                //     printf("Такой уже есть\n");
                //     break;
                // }
                // tree_node_t *node = create_node(/* 0, NULL, NULL, NULL,  */input, ' ');
                // root = add_to_tree(root, node);
                break;
            case 4:
                printf("Введите число: ");
                if (scanf("%d", &input) != 1)
                {
                    fgets(tmp, sizeof(tmp), stdin);
                    printf("Неверно введено число\n");
                    break;
                }
                fgets(tmp, sizeof(tmp), stdin);
                if (!tree_search(root_avl, input))
                {
                    printf("Такого нет\n");
                    break;
                }
                root_avl = tree_delete(root_avl, input);
                // if (!find_in_tree(root, input))
                // {
                //     printf("Такого нет!\n");
                //     break;
                // }
                // root = remove_in_tree(root, input);
                break;
            case 5:
                preorder_param(root_avl, avl_node_print, NULL);
                // prefix(root, node_print_int);
                printf("\n");
                break;
            case 6:
                inorder_param(root_avl, avl_node_print, NULL);
                // infix(root, node_print_int);
                printf("\n");
                break;
            case 7:
                postorder_param(root_avl, avl_node_print, NULL);
                // postfix(root, node_print_int);
                printf("\n");
                break;
            case 8:
                file = fopen("tree.dot", "w");
                if (!file)
                {
                    printf("Непредвиденная ошибка!\n");
                    break;
                }
                avl_export_to_dot(file, "OurTree", root_avl);
                // export_to_dot(file, "OurTree", root);
                fclose(file);
                file = NULL;
                break;
            case 9:
                // postfix(root, node_clear);
                postorder_param(root_avl, avl_node_clear, NULL);
                break;
            case 11:
                printf("Введите число: ");
                if (scanf("%d", &input) != 1)
                {
                    fgets(tmp, sizeof(tmp), stdin);
                    printf("Неверно введено число\n");
                    break;
                }
                fgets(tmp, sizeof(tmp), stdin);
                dummy = NULL;
                if (hash_table_find(open, input, &dummy) == 1)
                {
                    printf("Такого нет!\n");
                    break;
                }
                if (hash_table_remove(open, input))
                {
                    printf("Не удалось удалить элемент!\n");
                    break;
                }
                break;
            case 12:
                printf("Введите число: ");
                if (scanf("%d", &input) != 1)
                {
                    fgets(tmp, sizeof(tmp), stdin);
                    printf("Неверно введено число\n");
                    break;
                }
                fgets(tmp, sizeof(tmp), stdin);
                dummy = NULL;
                count = 0;
                if (hash_table_find_zad(open, input, &dummy, &count) == 1)
                {
                    printf("Такого нет!\n");
                    break;
                }
                else
                    printf("Искомое значение: %d, нужно было %d сравнений,его хаш: %zu\n", dummy->data, count, hash_of(dummy->data));
                break;
            case 13:
                printf("Введите число: ");
                if (scanf("%d", &input) != 1)
                {
                    fgets(tmp, sizeof(tmp), stdin);
                    printf("Неверно введено число\n");
                    break;
                }
                fgets(tmp, sizeof(tmp), stdin);
                // if (hash_table_find(open, input, &dummy) == 0)
                if (open_add(closed, input))
                {
                    printf("Такой уже есть!\n");
                    break;
                }
                break;
            case 14:
                printf("Введите число: ");
                if (scanf("%d", &input) != 1)
                {
                    fgets(tmp, sizeof(tmp), stdin);
                    printf("Неверно введено число\n");
                    break;
                }
                fgets(tmp, sizeof(tmp), stdin);
                // if (hash_table_remove(open, input))
                if (open_remove(closed, input))
                {
                    printf("Такого не было!\n");
                    break;
                }
                break;
            case 15:
                printf("Введите число: ");
                if (scanf("%d", &input) != 1)
                {
                    fgets(tmp, sizeof(tmp), stdin);
                    printf("Неверно введено число\n");
                    break;
                }
                fgets(tmp, sizeof(tmp), stdin);
                count = 0;
                // if (hash_table_find(open, input, &dummy) == 1)
                if (open_find_zad(closed, input, &count) == ERR_HASH)
                {
                    printf("Такого нет!\n");
                    break;
                }
                else
                    printf("Искомое значение: %d, нужно было %d сравнений, его хаш: %zu\n", input, count, hash_of(input));
                break;
            case 16:
                open_zad = hash_table_create();
                closed_zad = open_init();
                
                root_zad = NULL;
                root_avl_zad = NULL;

                // for (size_t j = 0; j < COUNT_TEST;)
                // {
                //     randint = rand();
                //     if (!find_in_tree(root_zad, randint))
                //     {
                //         tree_node_t *newnode = create_node(randint, randint);
                //         root_zad = add_to_tree(root_zad, newnode);
                //         root_avl_zad = tree_insert(root_avl_zad, randint);
                //         hash_set_insert(open, randint);
                //         open_add(closed, randint);
                //         j++;
                //     }
                // }
                for (size_t i = 0; i < COUNT_TEST; i++)
                {
                    tree_node_t *newnode = create_node(i, i);
                    root_zad = add_to_tree(root_zad, newnode);
                    root_avl_zad = tree_insert(root_avl_zad, i);
                    hash_set_insert(open_zad, i);
                    open_add(closed_zad, i);
                }
                count = 0;
                begin = microseconds_now();
                for (size_t i = 0; i < COUNT_TEST; i++)
                    find_in_tree_zad(root_zad, i, &count);
                end = microseconds_now();
                printf("Ср. Время на поиск любого эл-та в ДДП: %.2lf, ср. кол-во сравнений %.2lf\n", (double)(end - begin) / COUNT_TEST, (double)(count) / COUNT_TEST);

                count = 0;
                begin = microseconds_now();
                for (size_t i = 0; i < COUNT_TEST; i++)
                    tree_search_zad(root_avl_zad, i, &count);
                end = microseconds_now();
                // printf("Ср. Время на поиск любого эл-та в АВЛ: %lf\n", (double)(end - begin) / COUNT_TEST);
                printf("Ср. Время на поиск любого эл-та в АВЛ: %.2lf, ср. кол-во сравнений %.2lf\n", (double)(end - begin) / COUNT_TEST, (double)(count) / COUNT_TEST);

                count = 0;
                begin = microseconds_now();
                for (size_t i = 0; i < COUNT_TEST; i++)
                    hash_table_find_zad(open_zad, i, &dummy, &count);
                end = microseconds_now();
                // printf("Ср. Время на поиск любого эл-та в хеш-таблице (открытая): %lf\n", (double)(end - begin) / COUNT_TEST);
                printf("Ср. Время на поиск любого эл-та в хеш-таблице (открытая): %.2lf, ср. кол-во сравнений %.2lf\n", (double)(end - begin) / COUNT_TEST, (double)(count) / COUNT_TEST);

                count = 0;
                begin = microseconds_now();
                for (size_t i = 0; i < COUNT_TEST; i++)
                    open_find_zad(closed_zad, i, &count);
                end = microseconds_now();
                // printf("Ср. Время на поиск любого эл-та в хеш-таблице (закрытая): %lf\n", (double)(end - begin) / COUNT_TEST);
                printf("Ср. Время на поиск любого эл-та в хеш-таблице (закрытая): %.2lf, ср. кол-во сравнений %.2lf\n", (double)(end - begin) / COUNT_TEST, (double)(count) / COUNT_TEST);
                
                printf("ДДП: %zu\n", sizeof(tree_node_t) * COUNT_TEST);
                printf("АВЛ: %zu\n", sizeof(avl_node_t) * COUNT_TEST);
                printf("Хеш (закрытая): %zu\n", sizeof(list_node_t *) * closed_zad->capacity + sizeof(list_node_t) * closed_zad->size);
                printf("Хеш (открытая): %zu\n", sizeof(value_t) * open_zad->capacity);
                printf("Тесты проводились для %d упорядоченных чисел\n", COUNT_TEST);

                postfix(root_zad, node_clear);
                postorder_param(root_avl_zad, avl_node_clear, NULL);
                free_hash_set(open_zad);
                free_hash_table(closed_zad);
                root_zad = NULL;
                root_avl_zad = NULL;
                open_zad = NULL;
                closed_zad = NULL;
                break;
            case 10:
                printf("Введите число: ");
                if (scanf("%d", &input) != 1)
                {
                    fgets(tmp, sizeof(tmp), stdin);
                    printf("Неверно введено число\n");
                    break;
                }
                fgets(tmp, sizeof(tmp), stdin);
                dummy = NULL;
                if (hash_table_find(open, input, &dummy) == 0)
                {
                    printf("Такой уже есть!\n");
                    break;
                }
                if (hash_set_insert(open, input))
                {
                    printf("Не удалось вставить элемент!\n");
                    break;
                }
                break;
            case 17:
                if (!open)
                    break;
                printf("Для открытой адресации:\n");
                for (size_t i = 0; i < open->capacity; i++)
                {
                    printf(" %zu) ", i + 1);
                    if (open->table[i].is_initialized)
                    {
                        printf("Число: %d, хеш: %zu\n", open->table[i].data, hash(open->table[i].data, open->capacity));
                    }
                    else
                        printf("[ Пусто ]\n");
                }
                break;
            case 18:
                if (!closed)
                    break;
                printf("Для закрытой адресации:\n");
                for (size_t i = 0; i < closed->capacity; i++)
                {
                    printf(" %zu) ", i + 1);
                    const list_node_t *bucket = closed->table[i];
                    while (bucket)
                    {
                        printf("[%d, хеш: %zu] -> ", bucket->data, hash_of(bucket->data) % closed->capacity);
                        bucket = bucket->next;
                    }
                    printf("[NULL]\n");
                }
                break;
            default:
                break;
        }
    }

    return EXIT_SUCCESS;
}
