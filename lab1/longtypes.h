#ifndef TYPES_H__
#define TYPES_H__

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "error.h"

#define MANTIS_SIZE 36
#define MAX_EXPONENT_MODULE 100000

#pragma pack(push, 1)
typedef struct digit_t
{
    /*
     * Структура цифры
     * Является битовым полем, способным вместить числа от 0 до 15.
     * Упакована так, чтобы не работало выравнивание
     * */
    unsigned int field: 4;
    // char field;
} digit_t;
#pragma pack(pop)

//2-10 система счисления
typedef struct mantis_t
{
    /*
     * Структура мантиссы
     * Является основным хранилищем значащих цифр.
     * */
    digit_t digits[MANTIS_SIZE];
} mantis_t;

typedef struct real_t
{
    /*
     * Структура действительного числа
     *
     * Является мантиссой со знаком и экспонентой,
     * также размером самой мантиссы (исключая незначащие нули)
     */
    bool is_neg_mantis;
    mantis_t mantissa;
    int exponent;
    size_t size;
} real_t;

typedef struct int_t
{
    /*
     * Структура целого числа
     *
     * Является мантиссой со знаком,
     * также имеющей размер самой мантиссы (исключая незначащие нули)
     */
    bool is_neg;
    mantis_t mantissa;
    size_t size;
} int_t;


/**
 * @brief Функция для записи из строки в структуру Действительное длинное число
 *
 * @param[in] buffer Строка, в которой содержится число
 * @param[out] real Структура Действительного Длинного Числа
 * @param[in] size_mant Размер его мантиссы
 * @return Код ошибки
 */
int real_read(char *buffer, real_t *real, size_t size_mant);

/**
 * @brief Функция для записи из строки в структуру целое длинное число
 *
 * @param[in] buffer Строка, в которой содержится число
 * @param[out] real Структура целого Длинного Числа
 * @param[in] size_mant Размер его мантиссы
 * @return Код ошибки
 */
int int_read(char *buffer, int_t *integer, size_t size_mant);

/**
 * @brief Печать целого длинного числа
 *
 * @param[in] integer Длинное целое число
 */
void int_print(const int_t *integer);

/**
 * @brief Печать действительного длинного числа
 *
 * @param[in] real действительное целое число
 */
void real_print(const real_t *real);

/**
 * @brief Функция для проверки равенста мантиссы числа и нуля
 *
 * @param[in] Mantissa Мантисса числа
 * @param[in] size_mant Размер этой мантиссы (иключая незначащие нули)
 * @return Правдиво или ложно равенство
 */
bool is_zero(const mantis_t *mantissa, size_t size_mant);

/**
 * @brief Функция для превращение из действительного числа правильное. Переворачивает число с зада на перед.
 *
 * @param[in,out] real Действительное число
 * @param[in] size_mant Размер его мантиссы
 */
void normalizer_real(real_t *real, size_t size_mant);

#endif

