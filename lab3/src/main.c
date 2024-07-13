#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../inc/matrix.h"
#include "../inc/matrix_alloc.h"
#include "../inc/errors.h"
#include "../inc/matrix_operations.h"

static void print_menu(void)
{
    printf("__________________________________________________________________________\n");
    printf("*** Пункты Меню ***\n");
    printf(" 1) Задать размер матрицам (задается для всех двух сразу)\n");
    printf(" 2) Вывести все две матрицы\n");
    printf(" 3) Заполнить первую матрицу\n");
    printf(" 4) Заполнить вторую матрицу\n");
    printf(" 5) Посчитать результат сложения первых двух разреженных матриц и вывести результат\n");
    printf(" 6) Посчитать результат сложения первых двух обычных матриц и вывести результат\n");
    printf(" 7) Очистить все три матрицы\n");
    printf(" 8) Закончить работу программы\n");
    printf("--------------------------------------------------------------------------\n");
}

int main (void)
{
    errors_t rc;
    int operation = -1, rows = -1, cols = -1, count = 0, elem = 0;
    char temp[256];

    std_matrix_t A, B, C;
    spr_matrix_t D, E, F;
    A.rows = 0;
    B.rows = 0;
    D.rows = 0;
    E.rows = 0;
    A.A = NULL;
    B.A = NULL;
    C.A = NULL;
    D.A = NULL;
    D.IA = NULL;
    D.JA = NULL;
    E.A = NULL;
    E.IA = NULL;
    E.JA = NULL;
    F.A = NULL;
    F.IA = NULL;
    F.JA = NULL;

    while (operation != 8)
    {
        print_menu();
        printf("Введите пункт меню: ");
        if (scanf("%d", &operation) != 1)
        {
            fgets(temp, 2, stdin);
            printf("Вы неверно выбрали пункт меню\n");
            continue;
        }
        fgets(temp, 2, stdin);
        if (operation < 1 || operation > 8)
        {
            printf("Вы неверно выбрали пункт меню\n");
            continue;
        }

        switch (operation)
        {
            case 1:
                printf("Количество строк матрицы: ");
                if (scanf("%d", &rows) != 1)
                {
                    fgets(temp, 2, stdin);
                    printf("Вы неверно ввели количество строк\n");
                    break;
                }
                fgets(temp, 2, stdin);
                if (rows <= 0)
                {
                    printf("Вы неверно ввели количество строк\n");
                    break;
                }
                printf("Количество столбцов матрицы: ");
                if (scanf("%d", &cols) != 1)
                {
                    fgets(temp, 2, stdin);
                    printf("Вы неверно ввели количество столбцов\n");
                    break;
                }
                fgets(temp, 2, stdin);
                if (cols <= 0)
                {
                    printf("Вы неверно ввели количество столбцов\n");
                    break;
                }

                free_std_matrix(&A);
                free_std_matrix(&B);
                free_sparse_matrix(&D);
                free_sparse_matrix(&E);
                rc = std_matrix_alloc(&A, rows, cols);
                if (rc != EXIT_SUCCESSFUL)
                {
                    printf("Не хватает памяти\n");
                    break;
                }
                rc = std_matrix_alloc(&B, rows, cols);
                if (rc != EXIT_SUCCESSFUL)
                {
                    free_std_matrix(&A);
                    printf("Не хватает памяти\n");
                    break;
                }

                rc = sparse_matrix_alloc(&D, rows, cols);
                if (rc != EXIT_SUCCESSFUL)
                {
                    free_std_matrix(&A);
                    free_std_matrix(&B);
                    printf("Не хватает памяти\n");
                    break;
                }
                rc = sparse_matrix_alloc(&E, rows, cols);
                if (rc != EXIT_SUCCESSFUL)
                {
                    free_std_matrix(&A);
                    free_std_matrix(&B);
                    free_sparse_matrix(&D);
                    printf("Не хватает памяти\n");
                    break;
                }
                rows = -1, cols = -1;
                break;
            case 2:
                if (E.rows > 0 && D.rows > 0)
                {
                    dump_sparse_matrix(stdout, &D);
                    dump_sparse_matrix(stdout, &E);
                }
                else
                    printf("Пустые матрицы\n");
                break;
            case 3:
                if (A.rows == 0)
                {
                    printf("Нечего делать с ней, сначала укажите размеры!\n");
                    break;
                }
                printf("Введите количество вводимых элементов: ");
                if (scanf("%d", &count) != 1)
                {
                    fgets(temp, sizeof(temp), stdin);
                    printf("Вы неверно указали количество вводимых элементов\n");
                    break;
                }
                fgets(temp, sizeof(temp), stdin);
                if (count <= 0)
                {
                    printf("Вы неверно указали количество вводимых элементов\n");
                    break;
                }
                if (count > B.rows * B.cols)
                {
                    printf("Вы хотите записать чисел больше, чем есть места в матрице\n");
                    break;
                }
                for (int i = 0; i < count; i++)
                {
                    printf("===================\n");
                    printf("Индекс строки от %d до %d\n", 1, A.rows);
                    printf("Индекс столбца от %d до %d\n\n", 1, A.cols);
                    printf("Пример ввода: '1 2 3'. На 1 строку, 2 столбец будет вставлено число 3\n");
                    printf("Введите строку и столбец через пробел вместе со значением: ");
                    if (scanf("%d %d %d", &rows, &cols, &elem) != 3)
                    {
                        fgets(temp, sizeof(temp), stdin);
                        printf("Вы неверно указали индекс матрицы или число!\n");
                        break;
                    }
                    fgets(temp, sizeof(temp), stdin);
                    rows--;
                    cols--;
                    if (rows < 0 || rows >= A.rows || cols < 0 || cols >= A.cols)
                    {
                        printf("Вы неверно указали индекс матрицы!\n");
                        break;
                    }
                    rc = set_sparse_matrix(&D, rows, cols, elem);
                    if (rc != EXIT_SUCCESSFUL)
                    {
                        printf("Что-то пошло не так со вставкой элемента в матрицу разряженную\n");
                        break;
                    }
                    A.A[rows][cols] = elem;
                }
                clear_zeros(&D);
                break;
            case 4:
                if (B.rows == 0)
                {
                    printf("Нечего делать с ней, сначала укажите размеры!\n");
                    break;
                }
                printf("Введите количество вводимых элементов: ");
                if (scanf("%d", &count) != 1)
                {
                    fgets(temp, sizeof(temp), stdin);
                    printf("Вы неверно указали количество вводимых элементов\n");
                    break;
                }
                fgets(temp, sizeof(temp), stdin);
                if (count <= 0)
                {
                    printf("Вы неверно указали количество вводимых элементов\n");
                    break;
                }
                if (count > A.rows * A.cols)
                {
                    printf("Вы хотите записать чисел больше, чем есть места в матрице\n");
                    break;
                }
                for (int i = 0; i < count; i++)
                {
                    printf("===================\n");
                    printf("Индекс строки от %d до %d\n", 1, A.rows);
                    printf("Индекс столбца от %d до %d\n\n", 1, A.cols);
                    printf("Пример ввода: '1 2 3'. На 1 строку, 2 столбец будет вставлено число 3\n");
                    printf("Введите строку и столбец через пробел вместе со значением: ");
                    if (scanf("%d %d %d", &rows, &cols, &elem) != 3)
                    {
                        fgets(temp, sizeof(temp), stdin);
                        printf("Вы неверно указали индекс матрицы или число!\n");
                        break;
                    }
                    fgets(temp, sizeof(temp), stdin);
                    rows--;
                    cols--;
                    if (rows < 0 || rows >= A.rows || cols < 0 || cols >= A.cols)
                    {
                        printf("Вы неверно указали индекс матрицы!\n");
                        break;
                    }
                    rc = set_sparse_matrix(&E, rows, cols, elem);
                    if (rc != EXIT_SUCCESSFUL)
                    {
                        printf("Rc at set D in sparse = %d\n", rc);
                        break;
                    }
                    B.A[rows][cols] = elem;
                }
                clear_zeros(&E);
                break;
            case 5:
                if (D.rows == 0)
                {
                    printf("Нечего складывать!\n");
                    break;
                }
                rc = add_sparse_matrix(&D, &E, &F);
                if (rc != EXIT_SUCCESSFUL)
                {
                    printf("Rc at sparse add = %d\n", rc);
                    break;
                }
                // rc = add_std_matrix(&A, &B, &C);
                // if (rc != EXIT_SUCCESSFUL)
                // {
                //     printf("Rc at std add = %d\n", rc);
                //     break;
                // }
                clear_zeros(&F);
                dump_sparse_matrix(stdout, &F);
                free_std_matrix(&C);
                free_sparse_matrix(&F);
                break;
            case 7:
                free_sparse_matrix(&D);
                free_sparse_matrix(&E);
                free_sparse_matrix(&F);
                free_std_matrix(&A);
                free_std_matrix(&B);
                free_std_matrix(&C);
                break;
            case 8:
                free_sparse_matrix(&D);
                free_sparse_matrix(&E);
                free_sparse_matrix(&F);
                free_std_matrix(&A);
                free_std_matrix(&B);
                free_std_matrix(&C);
                break;
            case 6:
                if (A.rows <= 0 || D.rows <= 0)
                {
                    printf("Нечего замерять. Выберите размеры!\n");
                    break;
                }
                rc = add_std_matrix(&A, &B, &C);
                if (rc != EXIT_SUCCESSFUL)
                {
                    printf("Произошла непредвиденная ошибка при сложении обычных матриц\n");
                    break;
                }
                dump_std_matrix(stdout, &C);
                free_std_matrix(&C);
                break;

        }
    }

    return EXIT_SUCCESSFUL;
}
