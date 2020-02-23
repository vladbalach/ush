#include "ush.h"

static bool name_comand(char temp) {
    char check = temp >> 7;

    if ((temp > 47 && temp < 58) || (temp > 64 && temp < 91) || temp == '~')
        return 1;
    if ((temp > 96 && temp < 123) || temp == 46 || temp == '/' || temp == '.')
        return 1;
    if (temp == '+' || temp == '-' || temp == '_')
        return 1;
    if (check != 0)
        return 1;
    else
        return 0;
}

static char *create_str_comand(char *parsing) {
    int str_len;
    char *temp_str = 0;

    if (parsing == 0)
        return mx_strnew(0);
    str_len = mx_strlen(parsing) - 1;
    while (str_len > -1 && name_comand(parsing[str_len]))
        str_len--;
    if (str_len == -1)
        temp_str =  mx_strjoin("\r", parsing);
    else
        temp_str = mx_strdup(&parsing[str_len]);
    while (str_len > -1 && parsing[str_len] == ' ')
        str_len--;
    if (str_len == -1)
        temp_str[0] = '\r';
    else
        temp_str[0] = parsing[str_len];
    return temp_str;
}

char *mx_mini_parser_tab(char *parsing, t_info *info) {
    char *tem_str = create_str_comand(parsing);
    int pos = 0;
    char if_comand = 0;
    char *temp = 0;

    if (mx_strstr(tem_str, "/"))
        tem_str[0] = '1';
    if (tem_str[0] != 0 && tem_str[1] == '~') {
        pos = 0;
        temp = mx_strdup(&tem_str[1]);
        if_comand = tem_str[0];
        mx_strdel(&tem_str);
        mx_HOME(&temp, &pos, info);
        tem_str = mx_strjoin("\r", temp);
        mx_strdel(&temp);
        tem_str[0] = if_comand;
    }
    return tem_str;
}
