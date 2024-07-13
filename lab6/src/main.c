#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include "tree.h"
#include "stack.h"
#include "defines.h"

#define COUNT_TEST 10000

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
    printf(" 2) Посчитать с помощью стека\n");
    printf(" 3) Посчитать с помощью дерева\n");
    printf(" 4) Добавить в дерево узел\n");
    printf(" 5) Удалить из дерева узел\n");
    printf(" 6) Обойти введенное дерево префиксно\n");
    printf(" 7) Обойти введенное дерево инфиксно\n");
    printf(" 8) Обойти введенное дерево постфиксно\n");
    printf(" 9) Выгрузить дерево в файл\n");
    printf(" 10) Очистить дерево\n");
    printf(" 11) Посчитать эффективность\n");
    printf(" 12) Закончить работу программы\n");
    printf("\n---------------------------\n");
}


int main(void)
{
    int arr[NUMS_COUNT];
    tree_node_t *root = NULL;
    tree_node_t *tmp_node = NULL;
    list_stack_t stack_zad = { .top = NULL, .index = 0 };
    int oper = -1, res = 0, input = 0;
    int was_inputed = 0;
    int temp;
    unsigned long long begin, end;
    FILE *file = NULL;
    char tmp[256];
    srand(time(NULL));

    while (oper != 12)
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
            case 12:
                postfix(root, node_clear);
                break;
            case 1:
                printf("Введите %d целочисленных значений для A, B, C ... I: ", NUMS_COUNT);
                int i;
                for (i = 0; i < NUMS_COUNT; i++)
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
                if (i == NUMS_COUNT)
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
                res = stack_get_result(arr);
                printf("Результат: %d\n", res);
                break;
            case 3:
                if (!was_inputed)
                {
                    printf("Вы не ввели выражение\n");
                    break;
                }
                tree_node_t *root_zad = create_tree(arr);
                // postfix(root_zad, node_print);
                if (!root_zad)
                {
                    printf("Ошибка при выделении памяти\n");
                    break;
                }
                postfix_zad(root_zad, 0);
                printf("Результат: %d\n", root_zad->value);

                printf("Prefix: ");
                prefix(root_zad, node_print);
                printf("\n");
                printf("Infix: ");
                infix(root_zad, node_print);
                printf("\n");
                printf("Postfix: ");
                postfix(root_zad, node_print);
                printf("\n");
                
                print_tree(root_zad, 0);
                postfix(root_zad, node_clear);
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
                if (find_in_tree(root, input))
                {
                    printf("Такой уже есть\n");
                    break;
                }
                tree_node_t *node = create_node(/* 0, NULL, NULL, NULL,  */input, ' ');
                root = add_to_tree(root, node);
                break;
            case 5:
                printf("Введите число: ");
                if (scanf("%d", &input) != 1)
                {
                    fgets(tmp, sizeof(tmp), stdin);
                    printf("Неверно введено число\n");
                    break;
                }
                fgets(tmp, sizeof(tmp), stdin);
                if (!find_in_tree(root, input))
                {
                    printf("Такого нет!\n");
                    break;
                }
                root = remove_in_tree(root, input);
                break;
            case 6:
                prefix(root, node_print_int);
                printf("\n");
                break;
            case 7:
                infix(root, node_print_int);
                printf("\n");
                break;
            case 8:
                postfix(root, node_print_int);
                printf("\n");
                break;
            case 9:
                file = fopen("tree.dot", "w");
                if (!file)
                {
                    printf("Непредвиденная ошибка!\n");
                    break;
                }
                export_to_dot(file, "OurTree", root);
                fclose(file);
                file = NULL;
                break;
            case 10:
                postfix(root, node_clear);
                break;
            case 11:
                if (!was_inputed)
                {
                    printf("Не ввели выражение!\n");
                    break;
                }
                printf("Тесты проводились %d раз\n", COUNT_TEST);
                temp = 0;
                root_zad = create_tree(arr);
                begin = microseconds_now();
                while (temp++ < COUNT_TEST)
                    postfix_zad(root_zad, 0);
                end = microseconds_now();
                postfix(root_zad, node_clear);
                printf("Время для вычисления выражения по дереву: %f\n\n", (double)(end - begin) / COUNT_TEST);

                temp = 0;
                begin = microseconds_now();
                while (temp++ < COUNT_TEST)
                    stack_get_result(arr);
                end = microseconds_now();
                printf("Время для вычисления выражения по стеку: %f\n\n", (double)(end - begin) / COUNT_TEST);
                
                
                begin = microseconds_now();
                for (size_t i = 0; i < COUNT_TEST; i++)
                    l_push(&stack_zad, i);
                for (size_t i = 0; i < COUNT_TEST; i++)
                    l_pop(&stack_zad, &res);
                end = microseconds_now();
                printf("Время для заполнения и опустошения стека: %f\n\n", (double)(end - begin) / COUNT_TEST);

                root_zad = NULL;
                begin = microseconds_now();
                for (size_t i = 0; i < COUNT_TEST; i++)
                {
                    tmp_node = create_node(i, i);
                    if (tmp_node/* && ! !find_in_tree(root_zad, i)*/)
                        root_zad = add_to_tree(root_zad, tmp_node);
                }
                postfix(root_zad, node_clear);
                root_zad = NULL;
                end = microseconds_now();
                printf("Время для заполнения и опустошения дерева (максимально неразветленного): %f\n\n", (double)(end - begin) / COUNT_TEST);
                
                begin = microseconds_now();
                for (size_t j = 0; j < COUNT_TEST;)
                {
                    tmp_node = create_node(rand(), j);
                    if (tmp_node && !find_in_tree(root_zad, j))
                    {
                        root_zad = add_to_tree(root_zad, tmp_node);
                        j++;
                    }
                }
                postfix(root_zad, node_clear);
                end = microseconds_now();
                root_zad = NULL;
                printf("Время для заполнения (с проверками) и опустошения дерева (случайного): %f\n\n", (double)(end - begin) / COUNT_TEST);
                break;
            default:
                break;
        }
    }

    return EXIT_SUCCESS;
}
