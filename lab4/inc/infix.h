#ifndef INFIX_H__
#define INFIX_H__

#include "list_stack.h"
#include "array_stack.h"
#include "errors.h"

str_valids_t validate_infix(const char *infix);

char *convert_to_postfix_from_infix_list(char *infix, list_stack_t *stack, free_t *area);

char *convert_to_postfix_from_infix_array(char *infix, array_stack_t *stack);

#endif

