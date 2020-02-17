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

static bool flag_oper(char *str, int *pos, int flag) {
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

static int new_spec_symbol(char *str, int *pos, int end, int flag) {
    int new_flag = flag;

    if (if_symbol(str[*pos]) && mx_check_symbol(str, *pos, str[*pos])) {
        (*pos)++;
        new_flag = mx_end_flag(str, pos, end, new_flag);
    }
    else if (*pos > 0 && str[*pos] == 40 
             && mx_check_symbol(str, *pos - 1, '$')) {
        (*pos)++;
        new_flag = mx_end_flag(str, pos, end, ')');
    }
    return new_flag;
}

int mx_flang_Comand(char *str, int *pos, int end, int flag) {
    int check_flag = flag;

    while (*pos <= end) {
        check_flag = str[*pos];
        if (mx_check_symbol(str, *pos, flag) || flag_oper(str, pos, flag)) {
            if (check_flag == ')')
                (*pos)++;
            return 0;
        }
        else if ((if_symbol(str[*pos])
                  && mx_check_symbol(str, *pos, str[*pos]))
                  || (*pos > 0 && str[*pos] == 40 
             && mx_check_symbol(str, *pos - 1, '$')))
             check_flag = new_spec_symbol(str, pos, end, check_flag);
        else
            (*pos)++;
    }
    if (flag != 32)
        printerrflag(flag);
    return flag;
}
