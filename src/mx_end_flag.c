#include "ush.h"

static bool if_symbol(char test) {
    if (test == 34 || test == 39 || test == 96)
        return 1;
    return 0;
}

static void printerrflag(char flag) {
    mx_printerr("u$h: parse error near | ");
    mx_printcharerr(flag);
    mx_printerr(" |\n");
}

static bool flag_operator(char *str, int *pos, int flag) {
    if (flag != ' ')
        return 0;
    if (str[*pos] == '|' || str[*pos] == '<'
        || str[*pos] == '>' || str[*pos] == '&')
        if (mx_check_symbol(str, *pos, str[*pos])) {
            (*pos)--;
            return 1;
        }
    return 0;
}

static int flang_Comand(char *str, int *pos, int end, int flag) {
    int check_flag = flag;

    while (*pos < end) {
        check_flag = str[*pos];
        if (mx_check_symbol(str, *pos, flag) || flag_operator(str, pos, flag))
            return 0;
        else if (if_symbol(str[*pos]) 
                 && mx_check_symbol(str, *pos, str[*pos]) && (*pos)++) {
            check_flag = mx_end_flag(str, pos, end, check_flag);
        }
        else if ((int)pos > 0 && str[*pos] == 40 
                && mx_check_symbol(str, *pos - 1, '$')) {
            (*pos)++;
            check_flag = mx_end_flag(str, pos, end, ')');
        }
        else
            (*pos)++;
    }
    if (flag != ' ')
        printerrflag(flag);
    return flag;
}

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
        check_flag = flang_Comand(str, position, end, flag);
    (*position)++;
    return check_flag;
}
