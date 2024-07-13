#ifndef ERROR_H__
#define ERROR_H__

#define EXIT_SUCCESSFUL 0 //Все хорошо
#define ERR_RANGE 1       //Ошибка, связанная с вводом слишком больших значений
#define EMPTY 2           //Пусто
#define OVERFLOW_MAN 3    //Переполнение длины мантиссы
#define OVERFLOW_EXP 4    //Переполнение порядка экспоненты
#define ERR_IO 5          //Неправильный ввож
#define MACHINE_ZERO 6    //Машинный ноль (deprecated, NOW: >==- overflow_exp)
#define ERR_ARGS 7        //Ошибка с вводом аргументов командной строки

#endif

