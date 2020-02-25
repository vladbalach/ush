#include "ush.h"

static void mx_skip_spaces (char *str, int *start, int end) {
    while (str[*start] && *start < end) {
        if (str[*start] != ' ') {
            return;
        }
        (*start)++;
    }
}

static bool if_symbol(char test) {
    if (test == 34 || test == 39 || test == 96) 
        return 1;
    return 0;
}

static void end_argv(int *curr_pos, int end, char *str) {
    int pos = 0;

    while (mx_is_char(str[*curr_pos]) || (str[*curr_pos] == ' '
           && !mx_check_symbol(str, *curr_pos, str[*curr_pos]))) {
        if (if_symbol(str[*curr_pos])
            && mx_check_symbol(str, *curr_pos, str[*curr_pos])) {
            pos = (*curr_pos);
            (*curr_pos)++;
            mx_end_flag(str, curr_pos, end, str[pos]);
        }
        else if(str[*curr_pos] == '$' && str[(*curr_pos) + 1] == '(') {
            pos = (*curr_pos);
            *curr_pos += 2;
            mx_end_flag(str, curr_pos, end, ')');
        }
        else
            (*curr_pos)++;
    }
}

static void push_argv_in_list(char **comand, t_token *new_token) {
    char **meny_comand = 0;

    meny_comand = mx_strsplit(*comand, '\x0d');
    mx_strdel(comand);
    if (meny_comand) {
        mx_strarr_add_to_strarr(&new_token->value, &meny_comand);
        new_token->type = 1;
        new_token->priority = 10;
    }
}

t_token *mx_token_in_program(int *curr_pos, int end, char *str,
                                 t_info *processes) {
    int token_start = *curr_pos;
    t_token *new_token = mx_create_token(2,0,0);
    char *newValue = 0;
    char *temp = 0;

    while (mx_is_char(str[token_start])) {
        end_argv(curr_pos, end, str);
        newValue = mx_strndup(&str[token_start], *curr_pos - token_start);
        temp = mx_audit_str(newValue, processes, 0);
        mx_strdel(&newValue);
        mx_skip_spaces(str, curr_pos, end);
        token_start = *curr_pos;
        push_argv_in_list(&temp, new_token);
    }
    return new_token;
}
