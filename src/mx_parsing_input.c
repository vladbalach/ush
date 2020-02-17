#include "ush.h"

static bool if_symbol(char test) {
    if (test == 34 || test == 39 || test == 96)
        return 1;
    return 0;
}

static void check_end_comand(char *temp, int *pos, int end, int *flag) {
    if (temp[*pos + 1] == ';') {
        mx_printerr("u$h: parse error near \';;\'\n");
        *pos = end;
        *flag = 1;
    }
    else
        temp[*pos] = 0;
}


static int check_spec_symbol(char *st, int *pos, int end, int flag) {
    int new_flag = flag;

    if (mx_check_symbol(st, *pos, ';'))
        check_end_comand(st, pos, end, &new_flag);
    else if (if_symbol(st[*pos]) && mx_check_symbol(st, *pos, st[*pos]))
        new_flag = st[*pos];
    else if (*pos > 0 && st[*pos] == 40
             && mx_check_symbol(st, *pos - 1, '$'))
        new_flag = ')';
    return new_flag;
}

char *mx_parsing_input(char *str) {
    char *st = mx_strdup(str);
    int end = mx_strlen(st);
    int pos = 0;
    int flag = 0;

    while (pos <= end) {
        if (flag != 0)
            flag = mx_end_flag(st, &pos, end, flag);
        if (pos < end)
            flag = check_spec_symbol(st, &pos, end, flag);
        pos++;
    }
    if (flag != 0)
        mx_strdel(&st);
    return st;
}
