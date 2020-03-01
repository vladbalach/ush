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
    if (str[pos[0]] == '|' || str[pos[0]] == '<'
        || str[pos[0]] == '>' || str[pos[0]] == '&')
        if (mx_check_symbol(str, pos[0], str[pos[0]])) {
            pos[0]--;
            return 1;
        }
    return 0;
}

static int new_spec_symbol(char *str, int *pos, int end, int flag) {
    int new_flag = flag;

    if (if_symbol(str[pos[0]]) && mx_check_symbol(str, pos[0], str[pos[0]])) {
        pos[0]++;
        new_flag = mx_end_flag(str, pos, end, new_flag);
    } 
    else if (pos[0] > 0 && str[*pos] == 40 
             && mx_check_symbol(str, pos[0] - 1, '$')) {
        pos[0]++;
        new_flag = mx_end_flag(str, pos, end, ')');
    }
    return new_flag;
}

int mx_flang_comand(char *str, int *pos, int end, int flag) {
    int check_flag = flag;

    while (pos[0] <= end) {
        check_flag = str[pos[0]];
        if (mx_check_symbol(str, pos[0], flag) || flag_oper(str, pos, flag)) {
            return 0;
        }
        else if ((if_symbol(str[*pos])
                  && mx_check_symbol(str, pos[0], str[pos[0]]))
                  || (pos[0] > 0 && str[pos[0]] == 40 
             && mx_check_symbol(str, pos[0] - 1, '$')))
             check_flag = new_spec_symbol(str, pos, end, check_flag);
        else
            (pos[0])++;
    }
    if (flag != 32)
        printerrflag(flag);
    return flag;
}
