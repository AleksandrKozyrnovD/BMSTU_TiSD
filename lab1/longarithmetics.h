#ifndef LONGARITHMETICS_H__
#define LONGARITHMETICS_H__

#include "longtypes.h"
#include "error.h"

/**
 * @brief Функция для вычитания мантисс. Считается, что мантиссы одного порядка
 *
 * @param[in,out] left Левая мантисса
 * @param[in,out] size_left Размер левой мантиссы
 * @param[in] right Правая мантисса
 * @param[in] size_right Размер правой мантиссы
 */
void substract_mantis(mantis_t *left, size_t *size_left, const mantis_t *right, size_t size_right);

/**
 * @brief Сравнение беззнаковых мантисс. Считается, что мантиссы одного порядка
 *
 * @param[in] left Левая мантисса
 * @param[in] size_left Ее размер
 * @param[in] right Правая мантисса
 * @param[in] size_right Ее размер
 * @return Аналогично strcmp. Разница первых несовпадающих чисел (размеров мантиссы тоже).
 */
int cmp_mantiss(const mantis_t *left, int size_left, const mantis_t *right, int size_right);

/**
 * @brief Функция деления действительного длинного числа на короткое целое
 *
 * @param[out] result Результат деления
 * @param[in] real_num Действительное длинное число
 * @param[in] int_num Целое длинное число
 * @return код ошибки
 */
int divide_real_from_int(real_t *result, real_t real_num, int_t int_num);

#endif

