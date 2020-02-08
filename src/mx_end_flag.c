#include "ush.h"

static int flang_Comand(char *str, int *position, int end, int flag) {
    // mx_printstr("\nComand\n");
    int check_flag = flag;

    while (*position < end) {
        check_flag = str[*position];
        if (mx_check_symbol(str, *position, flag))
            return 0;
        else if (mx_check_symbol(str, *position, 34) || mx_check_symbol(str, *position, 96) || mx_check_symbol(str, *position, 39)) {
            (*position)++;
            check_flag = mx_end_flag(str, position, end, check_flag);
        }
        else if (position > 0 && str[*position] == 40 && mx_check_symbol(str, *position - 1, '$')){
            (*position)++;
            check_flag = mx_end_flag(str, position, end, ')');
        }
        else
            (*position)++;
    }
    mx_printerr("u$h: parse error near | ");
    mx_printcharerr(flag);
    mx_printerr(" |\n");
    return flag;
}

static int flang_String(char *str, int *position, int end, int flag) {
    // mx_printstr("\nString\n");
    while (*position < end) {
        if (mx_check_symbol(str, *position, 39))
            return 0;
        (*position)++;
    }
    mx_printerr("u$h: parse error near | \' |\n");
    return flag;
}


int mx_end_flag(char *str, int *position, int end, int flag) {
    int check_flag = 0;

    // mx_printstr("\n");
    // mx_printchar(flag);
    // mx_printstr("\n");
    if (flag == 39)
        check_flag = flang_String(str, position, end, flag);
    else
        check_flag = flang_Comand(str, position, end, flag);
    (*position)++;
    return check_flag;
}