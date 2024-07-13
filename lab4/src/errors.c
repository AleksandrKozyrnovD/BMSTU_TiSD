#include "errors.h"
#include <stdio.h>

void print_generic_valids(FILE *file, str_valids_t state)
{
    switch (state)
    {
        case VALID:
            break;
        case TWO_OPERANDS:
            fprintf(file, "Между двумя операндами не стоит операции!\n");
            break;
        case TWO_OPERATIONS:
            fprintf(file, "Две подряд операции считаются некорректным вводом!\n");
            break;
        case ONE_OPERATION:
            fprintf(file, "Моно-операция хоть и считается корректной, однако она вызывает больше проблем\n");
            break;
        case NO_OPERAND:
            fprintf(file, "После операции нет операнда!\n");
            break;
        default:
            fprintf(file, "Возникла непредвиденная проблема!\n");
            break;
    }
}

