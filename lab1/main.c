#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "longarithmetics.h"
#include "longtypes.h"


int main(int argc, char **argv)
{
    real_t real;
    int_t integer;
    int rc = EXIT_SUCCESSFUL;

    (void) argv;
    
    char buffer_real[256], buffer_int[256]; 

    if (argc >= 2)
    {
        printf("Укажите два числа таким образом:\napp.exe LongNum ShortNum\n");
        printf("LongNum записывается в виде: [+-]m.n[Ee][+-]k\n - 0 < m + n <= 35\n - |k| <= 99999\n");
        printf("ShortNum записывается в виде [+-]m\n - 0 < m <= 35\n");
        printf("Вывод программы записывается в виде [+-]0.mE[+-]k\n - 0 < m <= 35\n - |k| <= 99999\n");
        return EXIT_SUCCESSFUL;
    }

    printf("Введите вещественное число. Для помощи напишите ./app.exe -h\n");
    printf("1---5---10---15---20---25---30---35---|\n");
    printf("|---|----|----|----|----|----|----|---|\n");
    if (!fgets(buffer_real, sizeof(buffer_real), stdin))
    {
        printf("Случилась непредвиденная ошибка при чтении Вещественного числа\n");
        return ERR_IO;
    }
    printf("Введите целое число.\n");
    printf("1---5---10---15---20---25---30---35---|\n");
    printf("|---|----|----|----|----|----|----|---|\n");
    if (!fgets(buffer_int, sizeof(buffer_real), stdin))
    {
        printf("Случилась непредвиденная ошибка при чтении Целого числа\n");
        return ERR_IO;
    }

    if (buffer_real[strlen(buffer_real) - 1] == '\n')
    {
        buffer_real[strlen(buffer_real) - 1] = 0;
    }
    if (buffer_int[strlen(buffer_int) - 1] == '\n')
    {
        buffer_int[strlen(buffer_int) - 1] = 0;
    }
    
    memset(&real, 0, sizeof(real));
    memset(&integer, 0, sizeof(integer));
    if (strlen(buffer_real) == 0)
    {
        printf("Действительное Число введено неверно\n");
        return ERR_IO;
    }
    // if (strlen(buffer_real) >= 36)
    // {
    //     printf("Введено слишком длинное действительное число число\n");
    //     return ERR_IO;
    // }
    rc = real_read(buffer_real, &real, MANTIS_SIZE);
    switch (rc)
    {
        case EXIT_SUCCESSFUL:
            break;
        case ERR_IO:
            printf("Действительное Число введено неверно\n");
            return rc;
        case OVERFLOW_MAN:
            printf("Введено слишком действительное длинное число\n");
            return rc;
        default:
            printf("Случилась непредвиденная ошибка при чтении действительного числа\n");
            return rc;
    }
    
    // printf("\n");
    // real_print(&real);
    // printf("\n");
    
    if (strlen(buffer_int) == 0)
    {
        printf("Целое Число введено неверно\n");
        return ERR_IO;
    }
    if (strlen(buffer_int) >= 36)
    {
        printf("Введено слишком  длинное число\n");
        return ERR_IO;
    }
    rc = int_read(buffer_int, &integer, MANTIS_SIZE);
    switch (rc)
    {
        case EXIT_SUCCESSFUL:
            break;
        case ERR_IO:
            printf("Целое Число введено неверно\n");
            return rc;
        case OVERFLOW_MAN:
            printf("Введено слишком целое длинное число\n");
            return rc;
        default:
            printf("Случилась непредвиденная ошибка при чтении целого числа\n");
            return rc;
    }
    
    // int_print(&integer);
    // printf("\n");
    if (is_zero(&integer.mantissa, integer.size))
    {
        printf("Деление невозможно!\n");
        return ERR_ARGS;
    }
    
    if (is_zero(&real.mantissa, real.size))
    {
        printf("0.0e1\n");
        return EXIT_SUCCESSFUL;
    }

    rc = divide_real_from_int(&real, real, integer);
    
    switch (rc)
    {
        case EXIT_SUCCESSFUL:
            printf("Result: \n");
            printf("0.|---5---10---15---20---25---30---35---|\n");
            printf("0.|---|----|----|----|----|----|----|---|\n");
            real_print(&real);
            break;
        case OVERFLOW_EXP:
            printf("Экспонента слишком большая!\n");
            break;
        default:
            printf("Случилась непредвиденная ошибка при делении\n");
            break;
    }

    // real_print(&real);
    // printf("Rc = %d\n", rc);

    return rc;
}
