#include "infix.h"
#include "errors.h"
#include "list_stack.h"
#include "array_stack.h"
#include "oper.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

static int get_order(char c)
{
    switch (c)
    {
        case '+':
            return PLUS >> 1;
        case '-':
            return MINUS >> 1;
        case '*':
            return MULT >> 1;
        case '/':
            return DIV >> 1;
        case '^':
            return POWER >> 1;
        default:
            return NOT_OPER;
    };

    return NOT_OPER;
}

static operation_t convert_to_oper(char c)
{
    switch (c)
    {
        case '+':
            return PLUS;
        case '-':
            return MINUS;
        case '*':
            return MULT;
        case '/':
            return DIV;
        case '^':
            return POWER;
        default:
            return NOT_OPER;
    };

    return NOT_OPER;
}

static char convert_to_char(operation_t operation)
{
    switch (operation)
    {
        case PLUS:
            return '+';
        case MINUS:
            return '-';
        case MULT:
            return '*';
        case DIV:
            return '/';
        case POWER:
            return '^';
        default:
            return '#';
    }

    return '#';
}

char *convert_to_postfix_from_infix_list(char *infix, list_stack_t *stack, free_t *area)
{
    size_t i, j; //indexes of infix and postfixes

    size_t len = strlen(infix);
    char *postfix = calloc(len * 2 + 1, sizeof(char));
    if (!postfix)
        return NULL;
    
    for (i = 0, j = 0; i < len; i++)
    {
        if (infix[i] == ' ' || infix[i] == '\t')
            continue;

        if (isalnum(infix[i]))
            postfix[j++] = infix[i];
        else if (infix[i] == '(')
            l_push(stack, BRACKET);
        else if (infix[i] == ')')
        {
            if (stack->index == 0)
                postfix[j++] = ' ';
            while (stack->index != 0 && l_peek(stack, area) != BRACKET)
            {
                postfix[j++] = ' ';
                postfix[j++] = convert_to_char(l_pop(stack, area));
            }
            l_pop(stack, area); //deleting ')'
        }
        else if (convert_to_oper(infix[i]) != -1)
        {
            postfix[j++] = ' ';
            operation_t oper = get_order(infix[i]);
            while (stack->index != 0 && (l_peek(stack, area) >> 1) >= oper && l_peek(stack, area) != BRACKET)
            {
                postfix[j++] = convert_to_char(l_pop(stack, area));
                postfix[j++] = ' ';
            }
            l_push(stack, convert_to_oper(infix[i]));
        }
    }

    // postfix[j++] = ' ';
    while (stack->index != 0)
    {
        if (l_peek(stack, area) == BRACKET)
        {
            free(postfix);
            return NULL;
        }
        postfix[j++] = ' ';
        postfix[j++] = convert_to_char(l_pop(stack, area));
    }
    postfix[j] = 0;


    return postfix;
}

char *convert_to_postfix_from_infix_array(char *infix, array_stack_t *stack)
{
    size_t i, j; //indexes of infix and postfixes

    size_t len = strlen(infix);
    char *postfix = calloc(len * 2 + 1, sizeof(char));
    if (!postfix)
        return NULL;
    
    for (i = 0, j = 0; i < len; i++)
    {
        if (infix[i] == ' ' || infix[i] == '\t')
            continue;

        if (isalnum(infix[i]))
            postfix[j++] = infix[i];
        else if (infix[i] == '(')
            a_push(stack, BRACKET);
        else if (infix[i] == ')')
        {
            if (stack->beg == stack->curr)
                postfix[j++] = ' ';
            while (stack->beg != stack->curr && a_peek(stack) != BRACKET)
            {
                postfix[j++] = ' ';
                postfix[j++] = convert_to_char(a_pop(stack));
            }
            a_pop(stack); //deleting ')'
        }
        else if (convert_to_oper(infix[i]) != -1)
        {
            postfix[j++] = ' ';
            operation_t oper = get_order(infix[i]);
            while (stack->curr != stack->beg && (a_peek(stack) >> 1) >= oper && a_peek(stack) != BRACKET)
            {
                postfix[j++] = convert_to_char(a_pop(stack));
                postfix[j++] = ' ';
            }
            a_push(stack, convert_to_oper(infix[i]));
        }
    }

    while (stack->curr != stack->beg)
    {
        if (a_peek(stack) == BRACKET)
        {
            free(postfix);
            return NULL;
        }
        postfix[j++] = ' ';
        postfix[j++] = convert_to_char(a_pop(stack));
    }
    postfix[j] = 0;

    return postfix;
}

str_valids_t validate_infix(const char *infix)
{
    bool isoperand = false, isoperation = false;
    bool wasnum = false;
    char c;
    size_t len = strlen(infix);
    for (size_t i = 0; i < len; i++)
    {
        c = infix[i];
        if (isspace(c) && !isoperation && !isoperand)
            continue;
        else if (isspace(c) && isoperand)
        {
            isoperand = false;
            wasnum = true;
        }
        else if (isspace(c) && isoperation)
        {
            isoperation = false;
            wasnum = false;
        }
        // else if ((convert_to_oper(c) != -1 && convert_to_oper(c) != BRACKET) && isoperation)
        //     return ERR_IO;
        else if (convert_to_oper(c) != -1 && wasnum)
        {
            isoperation = true;
            isoperand = false;
            wasnum = false;
        }
        else if ((convert_to_oper(c) != -1 && c != '(') && !wasnum && i == 0)
            return ONE_OPERATION;
        else if ((convert_to_oper(c) != -1 && c != '(') && !wasnum)
            return TWO_OPERATIONS;
        else if (isalnum(c) && wasnum && !isoperand)
            return TWO_OPERANDS;
        else if (isalnum(c) && !isoperand)
        {
            isoperand = true;
            isoperation = false;
            wasnum = true;
        }
    }
    
    if (!wasnum)
        return NO_OPERAND;
    return VALID;
}

