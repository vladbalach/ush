#include "ush.h"

static int get_start_index(char *str) {
    int i = 0;
    while(str[i]) {
        if (str[i] != ' ') {
            return i;
        }
        i++;
    }
    return i;
}

static int pos_parametr(char *str, int *start) {
    int i = *start;

    while (str[i] != '=') {
        // (*start)++;
        i++;
    }
    if (str[i] == '=') {
        return i;
    }
    else 
        return 0;
}

void mx_create_parameter(char *str, int *start, int end, t_info *processes) {
    int pos = pos_parametr(str, start);
    char *argument = mx_strndup(&str[*start], pos - *start);
    char *parametr = NULL;

    *start = ++pos;
    mx_printstr(argument);
    mx_printstr("|\n");
    mx_printint(pos);
    mx_end_flag(str, &pos, end, ' ');
    mx_printint(pos);
    // int str_len = mx_get_char_index(&str[pos], ' ');
    // if (str_len == -1)
    //     parametr = mx_strdup(&str[pos]);
    // else
    parametr = mx_strndup(&str[*start], pos - *start);
    *start = pos;

    mx_printint(*start);
    mx_printstr(parametr);
    mx_printstr("|\n");
    pos = get_start_index(&str[*start]);
    if (pos != -1)
        *start += pos;
    check_new_parameter(str, start, end, processes);
}
