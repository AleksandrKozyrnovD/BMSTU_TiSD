#include "longarithmetics.h"
#include <stdlib.h>
#include <assert.h>
#include "error.h"
#include "longtypes.h"
#include <stdbool.h>
#include <string.h>

#define BASE 10

static void remove_order(mantis_t *mantis, size_t size)
{
    for (size_t i = 0; i < size - 1; i++)
        mantis->digits[MANTIS_SIZE - i - 1] = mantis->digits[MANTIS_SIZE - i - 2];
    mantis->digits[0].field = 0;
}

/**
 * @brief Функция для прибавки к мантиссе единицы
 *
 * @param[in,out] mantis Мантисса
 * @param[in] size Ее размер
 * @return Разница между старой экспонентой и новой (разница в данном случае 0 <= x <= 1)
 */
static int add_one(mantis_t *mantis, size_t size)
{
    bool is_excess = true;
    for (size_t i = 0; i < size && is_excess; i++)
    {
        if (mantis->digits[MANTIS_SIZE - i - 1].field + is_excess == 10)
        {
            mantis->digits[MANTIS_SIZE - i - 1].field = 0;
        }
        else
        {
            is_excess = false;
            mantis->digits[MANTIS_SIZE - i - 1].field++;
        }
    }
    if (is_excess && size < MANTIS_SIZE)
    {
        mantis->digits[MANTIS_SIZE - size - 1].field = is_excess;
        is_excess = false;
    }
    else if (is_excess)
    {
        mantis->digits[0].field = 1;
    }

    return is_excess;
}

/**
 * @brief Функция вычитания мантисс. Считается, что мантиссы одного порядка и левая мантисса больше правой по модулю. Иначе неопределенное поведение
 *
 * @param[in,out] left Левая мантисса
 * @param[in,out] size_left Размер левой мантиссы
 * @param[in] right Правая мантисса
 * @param[in] size_right Размер правой мантиссы
 */
void substract_mantis(mantis_t *left, size_t *size_left, const mantis_t *right, size_t size_right)
{
    assert((*size_left >= size_right));
    assert((size_right > 0));
    mantis_t result;
    memset(&result, 0, sizeof(result));

    // result.digits[MANTIS_SIZE - 1].field = 0;
    size_t size = size_right, i = 0, index;
    int size_res = 0;
    bool is_excess = false, is_zeros = true;

    for (; i < size; i++)
    {
        index = MANTIS_SIZE - i - 1;
        if (left->digits[index].field >= right->digits[index].field + is_excess)
        {
            result.digits[index].field = left->digits[index].field - right->digits[index].field - is_excess;
            is_excess = false;
        }
        else
        {
            result.digits[index].field = BASE + left->digits[index].field - right->digits[index].field - is_excess;
            is_excess = true;
        }
    }
    
    if (!is_excess)
    {
        while (i < *size_left)
        {
            index = MANTIS_SIZE - i - 1;
            result.digits[index].field = left->digits[index].field;
            i++;
            // size_res++;
        }
    }
    while (is_excess && i < *size_left)
    {
        index = MANTIS_SIZE - i - 1;
        if (left->digits[index].field == 0)
        {
            result.digits[index].field = 9;
            i++;
        }
        else
        {
            result.digits[index].field = left->digits[index].field - 1;
            is_excess = false;
        }
    }

    for (size_t i = *size_left; i != 0; i--)
    {
        index = MANTIS_SIZE - i;
        // printf("Index = %zu :: val = %d\n", index, result.digits[index].field);
        if (is_zeros && result.digits[index].field == 0)
            continue;
        else if (result.digits[index].field != 0 || !is_zeros)
        {
            is_zeros = false;
            size_res++;
        }
    }
    // printf("Index = %d :: val = %d\n", MANTIS_SIZE - 1, result.digits[MANTIS_SIZE - 1].field);
    // if (result.digits[MANTIS_SIZE - 1].field != 0)
    //     size_res++;

    *left = result;
    *size_left = size_res;
}

int cmp_mantiss(const mantis_t *left, int size_left, const mantis_t *right, int size_right)
{
    assert((left && right));
    int compare_score = size_left - size_right;

    for (int i = 0; i < size_left && compare_score == 0; i++)
    {
        compare_score = left->digits[MANTIS_SIZE - size_left + i].field - right->digits[MANTIS_SIZE - size_left + i].field;
    }

    return compare_score;
}

static mantis_t get_mantis(const real_t *real, size_t begin, size_t end)
{
    mantis_t result;
    memset(&result, 0, sizeof(result));
    // for (size_t i = 0; i < MANTIS_SIZE; i++)
    //     result.digits[i].field = 0;
    size_t index;
    size_t mantis_index = end - begin - 1;

    for (; begin < end - 1; begin++)
    {
        index = MANTIS_SIZE - real->size + begin ;
        result.digits[MANTIS_SIZE - mantis_index].field = real->mantissa.digits[index].field;
        // printf("Index = %zu :: in res index = %zu :: value = %d\n", index, MANTIS_SIZE - mantis_index, real->mantissa.digits[index].field);
        mantis_index--;
    }

    return result;
}

/**
 * @brief Функция вставки цифры в начало мантиссы. Считается, что длина мантисы позволяет вместить еще одну цифру.
 *
 * @param[in, out] mantis Мантисса, в которую производится вставка
 * @param[in] size Размер мантиссы
 * @param[in] digit Цифра, которая вставляется
 */
static void increase_order(mantis_t *mantis, size_t size, int digit)
{
    // printf("Mantis size = %zu\n", size);
    assert((0 <= digit && digit <= 9));
    assert((size < MANTIS_SIZE + 1));
    //assert digit < 10 etc
    // printf("Digit = %d\n", digit);
    for (size_t i = 0; i < size; i++)
    {
        mantis->digits[MANTIS_SIZE - size + i - 1].field = mantis->digits[MANTIS_SIZE - size + i].field;
    }
    mantis->digits[MANTIS_SIZE - 1].field = digit;
}

/**
 * @brief Функция для нормализации размера мантиссы. Итогом функции служит удаление незначащих нулей и изменение размера мантиссы.
 *
 * @param[in] mantis Мантисса
 * @param[in,out] size Размер мантиссы
 */
static void normalize_mantis(const mantis_t *mantis, size_t *size)
{
    for (size_t i = *size; i != 0; i--)
    {
        if (mantis->digits[MANTIS_SIZE - i].field == 0)
            (*size)--;
        else
            break;
    }
}

int divide_real_from_int(real_t *result, real_t real_num, int_t int_num)
{
    mantis_t mantis_res, mantis_denominator;
    digit_t digit;
    digit.field = 0;
    size_t index = 0, mantis_index = 0, size_mant = 0;
    
    memset(&mantis_res, 0, sizeof(mantis_res));
    memset(&mantis_denominator, 0, sizeof(mantis_denominator));
    // for (size_t i = 0; i < MANTIS_SIZE; i++)
    // {
    //     mantis_res.digits[i] = digit;
    //     mantis_denominator.digits[i] = digit;
    // }
    result->is_neg_mantis = !(real_num.is_neg_mantis == int_num.is_neg);
    
    // real_num.exponent -= real_num.size >= int_num.size ? int_num.size - 1 : real_num.size - 1;

    while (cmp_mantiss(&real_num.mantissa, real_num.size, &int_num.mantissa, int_num.size) < 0)
    {
        if (int_num.size >= MANTIS_SIZE)
            return ERR_IO;
        // real_num.exponent--;
        increase_order(&real_num.mantissa, real_num.size, 0);
        real_num.size++;
    }

    if (real_num.size >= int_num.size)
    {
        mantis_denominator = get_mantis(&real_num, 0, int_num.size + 1);
        size_mant = int_num.size;
    }
    else
    {
        mantis_denominator = get_mantis(&real_num, 0, real_num.size + 1);
        size_mant = real_num.size;
    }

    if (cmp_mantiss(&mantis_denominator, size_mant, &int_num.mantissa, int_num.size) < 0)
        real_num.exponent -= size_mant;
    else
        real_num.exponent -= size_mant - 1;

    // printf("Size mant = %zu\n", size_mant);
    
    size_mant = int_num.size;
    index = int_num.size;
    while (mantis_index < MANTIS_SIZE)
    {
        // printf("Index = %zu\n", mantis_index);
        if (index >= real_num.size && size_mant == 0)
        {
            break;
        }
        else if (cmp_mantiss(&mantis_denominator, size_mant, &int_num.mantissa, int_num.size) < 0)
        {
            if (index < real_num.size)
                increase_order(&mantis_denominator, size_mant++, real_num.mantissa.digits[MANTIS_SIZE - real_num.size + index++].field);
            else
                increase_order(&mantis_denominator, size_mant++, 0);

            normalize_mantis(&mantis_denominator, &size_mant);
            if (cmp_mantiss(&mantis_denominator, size_mant, &int_num.mantissa, int_num.size) < 0)
                increase_order(&mantis_res, ++mantis_index, 0);
        }
        else
        {
            normalize_mantis(&mantis_denominator, &size_mant);

            digit.field = 0;
            while (cmp_mantiss(&mantis_denominator, size_mant, &int_num.mantissa, int_num.size) >= 0)
            {
                digit.field++;
                substract_mantis(&mantis_denominator, &size_mant, &int_num.mantissa, int_num.size);
            }
            increase_order(&mantis_res, ++mantis_index, digit.field);
        }
    }

    if (mantis_index >= MANTIS_SIZE - 1 && size_mant != 0)
    {
        while (cmp_mantiss(&mantis_denominator, size_mant, &int_num.mantissa, int_num.size) < 0)
            increase_order(&mantis_denominator, size_mant++, 0);

        digit.field = 0;
        while (cmp_mantiss(&mantis_denominator, size_mant, &int_num.mantissa, int_num.size) >= 0)
        {
            digit.field++;
            substract_mantis(&mantis_denominator, &size_mant, &int_num.mantissa, int_num.size);
        }
    }
    if (mantis_res.digits[mantis_index - 1].field >= 5 && mantis_index == MANTIS_SIZE)
    {
        remove_order(&mantis_res, mantis_index);
        mantis_index--;
        add_one(&mantis_res, mantis_index);
    }
    else if (mantis_index == MANTIS_SIZE)
    {
        remove_order(&mantis_res, mantis_index--);
    }
    if (real_num.exponent >= MAX_EXPONENT_MODULE || real_num.exponent <= -MAX_EXPONENT_MODULE)
        return OVERFLOW_EXP;

    result->exponent = real_num.exponent;
    result->size = mantis_index;
    result->mantissa = mantis_res;
    
    // normalize_mantis(&result->mantissa, &result->size);

    // for (size_t i = 0; i < result->size; i++)
    // {
    //     if (result->mantissa.digits[MANTIS_SIZE - i - 1].field == 0)
    //         result->size--;
    //     else
    //         break;
    // }

    return EXIT_SUCCESSFUL;
}

