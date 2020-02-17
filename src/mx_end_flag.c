#include "ush.h"

static int flang_String(char *str, int *position, int end, int flag) {
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

    if (flag == 39)
        check_flag = flang_String(str, position, end, flag);
    else
        check_flag = mx_flang_Comand(str, position, end, flag);
    (*position)++;
    return check_flag;
}
