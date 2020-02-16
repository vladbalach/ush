#include "ush.h"

void mx_parsing(char *str, t_info *info) {
    int end = mx_strlen(str);
    char *temp = mx_parsing_input(str);
    char **comands = 0;

    if (temp == 0) {
        return;
    }
    if (mx_check_symbol(str, end -1, '\\')) {
        mx_strdel(&temp);
        mx_printerr("u$h: parse error near | \\ |\n");
        return;
    }
    comands = mx_create_comands(temp, end);
    mx_execute(comands, info);
    mx_del_strarr(&comands);
}
