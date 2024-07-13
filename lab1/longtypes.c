#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "longtypes.h"
#include "error.h"

/**
 * @brief Функция для превращение из целого числа правильное. Переворачивает число с зада на перед.
 *
 * @param[in,out] integer Целое число
 * @param[in] size_mant Размер его мантиссы
 */
// static void normalizer_int(int_t *integer, size_t size_mant)
// {
//     digit_t digit;
//     for (size_t i = 0; i < integer->size / 2; i++)
//     {
//         digit = integer->mantissa.digits[size_mant - i - 1];
//         integer->mantissa.digits[size_mant - i - 1] = integer->mantissa.digits[size_mant - integer->size + i];
//         integer->mantissa.digits[size_mant - integer->size + i] = digit;
//     }
//
//     bool is_zeros = true;
//     size_t k = 0;
//     for (size_t i = 0; i < integer->size; i++)
//     {
//         // printf("\nNums_: %d :: %d\nIndex: %zu :: %zu\n===================", real->mantissa.digits[size_mant - i - 1].field, real->mantissa.digits[size_mant - i - k - 1].field, size_mant - i - 1, size_mant - i + k - 1);
//         if (integer->mantissa.digits[size_mant - i - 1].field != 0 || !is_zeros)
//         {
//             integer->mantissa.digits[size_mant - i + k - 1] = integer->mantissa.digits[size_mant - i - 1];
//             is_zeros = false;
//         }
//         else
//             k++;
//     }
//     integer->size -= k;
// }

int int_read(char *buffer, int_t *integer, size_t size_mant)
{
    size_t index = 0, len = strlen(buffer);
    integer->is_neg = false;
    bool is_zeros = true, is_signed = false;
    
    if (buffer[len - 1] == '\n')
    {
        is_signed = true;
        len--;
        buffer[len] = 0;
    }

    if (buffer[0] == '-')
    {
        is_signed = true;
        index++;
        integer->is_neg = true;
    }
    else if (buffer[0] == '+')
        index++;
    
    while (buffer[index] != 0)
    {
        // printf("Index = %zu :: Char = %c\n", index, buffer[index]);
        if (isdigit(buffer[index]))
        {
            if (buffer[index] - '0' != 0 || !is_zeros)
            {
                is_zeros = false;
                integer->size++;
                if (integer->size > size_mant)
                    return OVERFLOW_MAN;
                integer->mantissa.digits[size_mant - len + is_signed + integer->size - 1].field = buffer[index] - '0';
            }
        }
        else
            return ERR_IO;
        index++;
    }
    
    // normalizer_int(integer, size_mant);

    return EXIT_SUCCESSFUL;
}
void int_print(const int_t *integer)
{
    if (integer->is_neg)
        printf("-");
    for (size_t i = 0; i < integer->size; i++)
        printf("%d", integer->mantissa.digits[MANTIS_SIZE - integer->size + i].field);
}

void normalizer_real(real_t *real, size_t size_mant)
{
    digit_t digit;
    for (size_t i = 0; i < real->size / 2; i++)
    {
        digit = real->mantissa.digits[size_mant - i - 1];
        real->mantissa.digits[size_mant - i - 1] = real->mantissa.digits[size_mant - real->size + i];
        real->mantissa.digits[size_mant - real->size + i] = digit;
    }

    bool is_zeros = true;
    size_t k = 0;
    for (size_t i = 0; i < real->size; i++)
    {
        // printf("\nNums_: %d :: %d\nIndex: %zu :: %zu\n===================", real->mantissa.digits[size_mant - i - 1].field, real->mantissa.digits[size_mant - i - k - 1].field, size_mant - i - 1, size_mant - i + k - 1);
        if (real->mantissa.digits[size_mant - i - 1].field != 0 || !is_zeros)
        {
            real->mantissa.digits[size_mant - i + k - 1] = real->mantissa.digits[size_mant - i - 1];
            is_zeros = false;
        }
        else
            k++;
    }
    real->size -= k;


}

int real_read(char *buffer, real_t *real, size_t size_mant)
{
    size_t index = 0, len = strlen(buffer);
    int exponent = 0;
    real->exponent = 0;
    real->is_neg_mantis = false;
    bool is_zeros = true, was_exponent = false, was_sign = false;
    bool all_zero = true;
    
    if (buffer[len - 1] == '\n')
    {
        len--;
        buffer[len] = 0;
    }


    if (buffer[0] == '-')
    {
        index++;
        real->is_neg_mantis = true;
    }
    else if (buffer[0] == '+')
        index++;

    for (size_t i = 0; i < len; i++)
    if ((buffer[i] != '0' && buffer[i] != '.') && all_zero)
    {
        all_zero = false;
    }
    
    if (all_zero)
    {
        real->size = 1;
        real->exponent = 1;
        real->mantissa.digits[0].field = 0;

        return EXIT_SUCCESSFUL;
    }

    for (size_t i = 0 + index; i < len; i++)
    {
        if (!isdigit(buffer[i]) && buffer[i] != '.' && buffer[i] != 'e' && buffer[i] != 'E')
        {
            if ((buffer[i] == '+' || buffer[i] == '-') && !was_sign)
                was_sign = true;
            else
                return ERR_IO;
        }
    }

    while (buffer[index] != 0 && buffer[index] != 'E' && buffer[index] != 'e' && buffer[index] != '.')
    {
        // printf("Index = %zu :: Char = %c\n", index, buffer[index]);
        if (isdigit(buffer[index]))
        {
            if (buffer[index] - '0' != 0 || !is_zeros)
            {
                was_exponent = true;
                is_zeros = false;
                real->size++;
                real->exponent++;
                if (real->size > size_mant)
                    return OVERFLOW_MAN;
                real->mantissa.digits[size_mant - real->size].field = buffer[index] - '0';
            }
        }
        else
            return ERR_IO;
        index++;
    }
    
    // printf("Before . is read\n");
    if (buffer[index] == 0)
        if (real->size == 0)
            return ERR_IO;
        else
        {
            normalizer_real(real, size_mant);
            return EXIT_SUCCESSFUL;
        }
    else if (buffer[index] == 'e' || buffer[index] == 'E')
    {
        index++;
        // printf("In 1 sscanf:\nIndex = %zu :: Char = %c\n", index, buffer[index]);
        if (sscanf(buffer + index, "%d", &exponent) != 1)
            return ERR_IO;
        // printf("exp = %d, realexp = %d\n", exponent, real->exponent);
        real->exponent += exponent;

    }
    else if (buffer[index] == '.')
    {
        index++;
        while (buffer[index] != 0 && buffer[index] != 'e' && buffer[index] != 'E')
        {
            // printf("Index = %zu :: Char = %c\n", index, buffer[index]);
            if (isdigit(buffer[index]))
            {
                if (buffer[index] - '0' != 0 || !is_zeros)
                {
                    is_zeros = false;
                    real->size++;
                    if (real->size > size_mant)
                        return OVERFLOW_MAN;
                    real->mantissa.digits[size_mant - real->size].field = buffer[index] - '0';
                }
                else if (!was_exponent)
                {
                    real->exponent--;
                }
            }
            else
                return ERR_IO;
            index++;
        }
    }
    if (buffer[index] == 0)
        if (real->size == 0)
            return ERR_IO;
        else
        {
            normalizer_real(real, size_mant);
            return EXIT_SUCCESSFUL;
        }
    else if (buffer[index] == 'e' || buffer[index] == 'E')
    {
        index++;
        // printf("In sscanf:\nIndex = %zu :: Char = %c\n", index, buffer[index]);
        if (sscanf(buffer + index, "%d", &exponent) != 1)
            return ERR_IO;
        // printf("exp = %d, realexp = %d\n", exponent, real->exponent);
        real->exponent += exponent;
    }
    
    normalizer_real(real, size_mant);

    if (real->size > MANTIS_SIZE - 1)
        return OVERFLOW_MAN;
    
    return EXIT_SUCCESSFUL;
}

void real_print(const real_t *real)
{
    if (real->is_neg_mantis)
        printf("-");
    printf("0.");
    for (size_t i = 0; i < real->size; i++)
    {
        // printf("%d", real->mantissa.digits[MANTIS_SIZE - 1 - i].field);
        // printf("Size = %d, i = %zu, val = %d\n", MANTIS_SIZE, i, real->mantissa.digits[MANTIS_SIZE - real->size + i].field);
        printf("%d", real->mantissa.digits[MANTIS_SIZE - real->size + i].field);
    }
    printf("e%d\n", real->exponent);
}

bool is_zero(const mantis_t *mantissa, size_t size_mant)
{
    for (size_t i = 0; i < size_mant; i++)
        if (mantissa->digits[MANTIS_SIZE - i - 1].field != 0)
            return false;

    return true;
}

