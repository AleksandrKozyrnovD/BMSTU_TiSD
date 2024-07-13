#include "adjmat.h"
#include "combinatorics.h"
#include "utils.h"
#include <sys/time.h>

unsigned long long microseconds_now(void)
{
    struct timeval value;

    if (gettimeofday(&value, NULL))
        return (unsigned long long) -1;
    
    return value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

int main(void)
{
    int ec;
    int size;
    adjmat_t *result = NULL;

    // welcome();

    printf("Введите число вершин графа: ");
    ec = rnginput(&size, 1, INT_MAX);
    if (ec)
    {
        printf("Введено недопустимое значение! Повторите попытку.\n");
        return ec;
    }

    adjmat_t *matrix = amcreate(size);
    // printf("Введите рёбра графа в формате 'V1 V2' (нумерация вершин начинается с 0).\n"
    //        "Для завершения ввода введите '-1' :\n");
    printf("Ввод совершать парами чисел. Пара чисел - это номера вершин графа, счет начинается от 1.\nДля окончания ввода написать 0\n");
    ec = amfill(matrix);
    if (ec)
    {
        if (ec == EINVALIDINTEGER || ec == EINVALIDRANGE)
        {
            printf("Введено недопустимое значение! Повторите попытку.\n");
            amfree(matrix);
            return ec;
        }
        if (ec == EINVALIDVERTEXPAIR)
        {
            printf("%s\n", "Путь в себя невозможен!");
            amfree(matrix);
            return ec;
        }
    }

    uint64_t start, end, time, temp = 0;
    start = microseconds_now();
    while (temp++ < 1000)
    {
        result = cutgraph(*matrix);
        amfree(result);
        result = NULL;
    }
    end = microseconds_now();
    time = end - start;
    
    result = cutgraph(*matrix);
    if (result)
    {
        printf("Удаленные рёбра графа отмечены красным цветом.\n");
        gvexport(*matrix, *result);
        printf("Время выполнения алгоритма: %.2lf\n", (double)time / 1000);
        amfree(matrix);
        amfree(result);
        return EOK;
    }
    printf("Невозможно сделать граф несвязным!\n");
    // amfree(result);
    // amfree(matrix);
    if (result)
    {
        amfree(result);
    }
    if (matrix)
        amfree(matrix);

    return EOK;
}
