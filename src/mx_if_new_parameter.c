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

void create_parameter(char *str, int *start, int end, t_info *processes) {
    int pos = pos_parametr(str, start);
    char *name = mx_strndup(&str[*start], pos - *start);
    char *value = NULL;
    char *temp = NULL;

    *start = ++pos;

    mx_end_flag(str, &pos, end, ' ');
    // mx_printint(pos);
    temp = mx_strndup(&str[*start], pos - *start);
    *start = pos;
    value = mx_audit_str(temp, processes, 0);

    mx_strdel(&temp);
    // mx_printstr(name);
    // mx_strdel(&name);
    // mx_printstr("|\n");
    // mx_printstr(value);
    // mx_strdel(&value);
    mx_serch_list(&(processes->var_tree), name, value);
    // mx_printstr("|\n");
    pos = get_start_index(&str[*start]);
    if (pos != -1) 
        *start += pos;
    mx_if_new_parameter(str, start, end, processes);
}

static bool is_not_operator(char c) {
    if (c == '|' || c == '&' || c == '>' || c == '<'|| c == '$' || c == ' ')
        return false;
    if (c == '='|| c == 92 || c == 34 || c == 39 || c== 96 || c == 0)
        return false;
    return true;
}

void mx_if_new_parameter(char *str, int *start, int end, t_info *processes) {
    int check_start = *start;

    while (is_not_operator(str[check_start]))
        check_start++;
    if (str[check_start] == '=') {
        create_parameter(str, start, end, processes);
    }
}