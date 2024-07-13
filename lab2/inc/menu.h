#ifndef MENU_H__
#define MENU_H__

#include <stdio.h>
#include "err.h"
#include "litra.h"
#include "myarray.h"

typedef enum action_t
{
    exit_menu,
    load,
    show_not_sorted,
    add_end,
    delete_by_title_first,
    print_by_keys,
    print_table_by_table,
    print_table_by_sorted_keys,
    print_roman_author,
    get_time_sort
} action_t;

/**
 * @brief Печать меню помощи
 */
void help_print(void);

#endif

