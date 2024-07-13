#include "errors.h"
#include "list_stack.h"
#include "array_stack.h"
#include "infix.h"
#include <stdio.h>
#include <stdlib.h>

static void help(void)
{
    printf("\n============================\n");
    printf(" 1) Добавить элемент в стак-лист\n");
    printf(" 2) Удалить элемент в стак-листе\n");
    printf(" 3) Вывести текущий элемент в стак-листе на его вершине (Побочный эффект: он будет добавлен в осв. область)\n");
    printf(" 4) Посмотреть область освобожденных данных\n");
    printf(" 5) Добавить элемент в стак-массив\n");
    printf(" 6) Удалить элемент в стак-массиве\n");
    printf(" 7) Вывести текущий элемент в стак-массиве\n");
    printf(" 8) Очистить стак-лист\n");
    printf(" 9) Очистить стак-массив\n");
    printf(" 10) Превратить инфиксную запись в постфиксную с помощью стак-листа (Побочный эффект: старые значения стака будут очищены!)\n");
    printf(" 11) Превратить инфиксную запись в постфиксную с помощью стак-массива (Побочный эффект: старые значения стака будут очищены!)\n");
    printf(" 12) Очистить освобожденную область\n");
    printf(" 13) Вывести стак-лист\n");
    printf(" 14) Вывести стак-массив\n");
    printf(" 15) Прекратить работу программы\n");
    printf("============================\n");
}

int main (void)
{
    errors_t rc = EXIT_SUCCESSFUL;
    char tmp[256];
    char infix[256];
    char *postfix; //dynamic string
    str_valids_t valid;

    list_stack_t stack_list = { .top = NULL, .index = 0 };
    free_t area = { .head = NULL, .tail = NULL };
    
    array_stack_t stack_array = { .beg = NULL, .curr = NULL };
    rc = array_stack_init(&stack_array, 1);
    if (rc)
    {
        printf("Не хватило памяти!\n");
        return rc;
    }

    int choice = -1, number;

    while (choice != 15)
    {
        help();

        printf("\nВведите пункт меню: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nВы неверно указали пункт меню!\n");
            fgets(tmp, sizeof(tmp), stdin);
            choice = -1;
            continue;
        }
        fgets(tmp, sizeof(tmp), stdin);

        switch (choice)
        {
            case 1:
                printf("Введите целое число: ");
                if (scanf("%d", &number) != 1)
                {
                    printf("\nВы неверно указали целое число!\n");
                    fgets(tmp, sizeof(tmp), stdin);
                    continue;
                }
                fgets(tmp, sizeof(tmp), stdin);
                l_push(&stack_list, number);
                break;
            case 2:
                l_pop(&stack_list, &area);
                // printf("Удаление прошло успешно\n");
                break;
            case 3:
                if (stack_list.index != 0)
                    printf("Текущее число на вершине стака равно %d\n", l_peek(&stack_list, &area));
                else
                    printf("Ничего нет, стак пуст\n");
                break;
            case 4:
                if (area.head != NULL)
                    print_area(stdout, &area);
                else
                    printf("Ничего нет\n");
                break;
            case 5:
                printf("Введите целое число: ");
                if (scanf("%d", &number) != 1)
                {
                    printf("\nВы неверно указали целое число!\n");
                    fgets(tmp, sizeof(tmp), stdin);
                    continue;
                }
                fgets(tmp, sizeof(tmp), stdin);
                a_push(&stack_array, number);
                break;
            case 6:
                a_pop(&stack_array);
                break;
            case 7:
                if (stack_array.beg != stack_array.curr)
                    printf("Текущее число на вершине стака равно %d\n", a_peek(&stack_array));
                else
                    printf("Ничего нет, стак пуст\n");
                break;
            case 8:
                free_l_stack(&stack_list, &area);
                break;
            case 9:
                free_a_stack(&stack_array);
                rc = array_stack_init(&stack_array, 1);
                if (rc)
                {
                    printf("Произошла непредвиденная ошибка\n");
                    break;
                }
                break;
            case 10:
                printf("\nВалидными именами могут быть как и числа, так и символы, так и все вместе.\nНапример, 'A', 'bca', '10a', 'abc110abc'\n\n");
                printf("Введите инфиксную запись:\n");
                if (!fgets(infix, sizeof(infix), stdin))
                {
                    printf("Произошла ошибка считывания инфиксной записи!\n");
                    break;
                }
                valid = validate_infix(infix);
                if (valid)
                {
                    print_generic_valids(stdout, valid);
                    continue;
                }
                free_l_stack(&stack_list, &area);
                postfix = convert_to_postfix_from_infix_list(infix, &stack_list, &area);
                if (postfix)
                {
                    printf("Постфиксная запись:\n%s\n", postfix);
                }
                else
                {
                    printf("Случилась ошибка при превращении из инфиксной в постфиксную форму!\n");
                    break;
                }
                free(postfix);
                break;
            case 11:
                printf("\nВалидными именами могут быть как и числа, так и символы, так и все вместе.\nНапример, 'A', 'bca', '10a', 'abc110abc'\n\n");
                printf("Введите инфиксную запись:\n");
                if (!fgets(infix, sizeof(infix), stdin))
                {
                    printf("Произошла непредвиденная ошибка считывания инфиксной записи!\n");
                    break;
                }
                valid = validate_infix(infix);
                if (valid)
                {
                    print_generic_valids(stdout, valid);
                    continue;
                }
                free_a_stack(&stack_array);
                rc = array_stack_init(&stack_array, 2);
                if (rc)
                {
                    printf("Случилась непредвиденная ошибка\n");
                    break;
                }
                postfix = convert_to_postfix_from_infix_array(infix, &stack_array);
                if (postfix)
                {
                    printf("Постфиксная запись:\n%s\n", postfix);
                }
                else
                {
                    printf("Случилась непредвиденная ошибка при превращении из инфиксной в постфиксную форму!\n");
                    break;
                }
                free(postfix);
                break;
            case 12:
                free_area(&area);
                break;
            case 13:
                l_print(stdout, &stack_list, &area);
                break;
            case 14:
                a_print(stdout, &stack_array);
                break;
            case 15:
                free_l_stack(&stack_list, &area);
                free_a_stack(&stack_array);
                free_area(&area);
                break;
            default:
                break;
        }
    }

    return rc;
}
