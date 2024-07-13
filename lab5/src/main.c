#include "app.h"
#include "io.h"
#include "array.h"
#include "timemem.h"
#include "list.h"

int main(void)
{
    setbuf(stdout, NULL);
    srand(time(NULL));
    int n = 1000;
    int interval = 100;
    int flag = 0;

    //Начальные значения
    times_t t1 = { 1, 5 };
    times_t t2 = { 0, 3 };
    times_t t3 = { 0, 4 };
    times_t t4 = { 0, 1 };

    int num_command = -1;

    while (num_command != 0)
    {
        print_hello();

        // if (check_number(&num_command, 0, 4) != OK)
        if (scanf("%d", &num_command) != 1)
        {
            clean_my();
            printf("Введена некорректная команда, попробуйте снова\n");
            // break;
            continue;
        }
        else if (num_command < 0 || num_command > 4)
        {
            clean_my();
            printf("Введена некорректная команда, попробуйте снова\n");
            // break;
            continue;
        }
        else
        {
            fflush (stdin);
            switch (num_command)
            {
                case 1:
                    // printf("Симуляция для очереди в виде массива.\n");
                    sd_array(n, interval, t1, t2, t3, t4);//, flag);
                    break;
                case 2:
                    // printf("Симуляция для очереди в виде списка.\n");
                    printf("Выводить информация о памяти? \n1 - да, 0 - нет\nВыбор: ");
                    int rc = scanf("%d", &flag);
                    if (rc != 1 || !(flag == 1 || flag == 0))
                    {
                        printf("Некорректный выбор!\n");
                        break;
                    }
                    sd_list(n, interval, t1, t2, t3, t4, flag);
                    break;
                case 3:
                    printf("Изменение времени обработки. (введите цифру для изменения времени)\n");
                    double l, r;
                    int c;

                    printf("\nВремя прибытия\n");
                    printf("1 - T1: min = %lf; max = %lf\n", t1.min, t1.max);
                    printf("2 - T2: min = %lf; max = %lf\n", t2.min, t2.max);
                    printf("\nВремя обработки\n");
                    printf("3 - T1: min = %lf; max = %lf\n", t3.min, t3.max);
                    printf("4 - T2: min = %lf; max = %lf\n", t4.min, t4.max);
                    printf("Какой интервал изменить?\n");
                    if (check_number(&c, 1, 4) != OK)
                    {
                        printf("Введён некорректный номер!\n");
                        break;
                    }
                    else
                    {
                        printf("Введите левую и правую границы: ");
                        int rc = scanf("%lf", &l);
                        if (rc != 1 || l < 0)
                        {
                            clean_my();
                            printf("Такого интервала нет!\n");
                            break;
                        }
                        rc = scanf("%lf", &r);
                        if (rc != 1 || r < 0)
                        {
                            clean_my();
                            printf("Такого интервала нет!\n");
                            break;
                        }
                        switch (c)
                        {
                            case 1:
                                t1.min = l;
                                t1.max = r;
                                break;
                            case 2:
                                t2.min = l;
                                t2.max = r;
                                break;
                            case 3:
                                t3.min = l;
                                t3.max = r;
                                break;
                            case 4:
                                t4.min = l;
                                t4.max = r;
                                break;
                        }
                        printf("После изменений\n");
                        printf("\nВремя прибытия\n");
                        printf("1 - T1: min = %lf; max = %lf\n", t1.min, t1.max);
                        printf("2 - T2: min = %lf; max = %lf\n", t2.min, t2.max);
                        printf("\nВремя обработки\n");
                        printf("3 - T1: min = %lf; max = %lf\n", t3.min, t3.max);
                        printf("4 - T2: min = %lf; max = %lf\n", t4.min, t4.max);
                        printf("Какой интервал изменить?\n");
                    }
                    break;
                case 4:
                    printf("Вывод сравнений по времени\n");
                    memory_io();
                    break;
            }
        }
    }
    return OK;
}

