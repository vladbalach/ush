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

static void end_argv(int *currPos, int end, char *str) {
    int pos = 0;

    while (mx_is_char(str[*currPos]) || (str[*currPos] == ' ' 
           && !mx_check_symbol(str, *currPos, str[*currPos]))) {
        if (if_symbol(str[*currPos])
            && mx_check_symbol(str, *currPos, str[*currPos])) {
            pos = (*currPos);
            (*currPos)++;
            mx_end_flag(str, currPos, end, str[pos]);
        }
        else if(str[*currPos] == '$' && str[(*currPos) + 1] == '(') {
            pos = (*currPos);
            *currPos += 2;
            mx_end_flag(str, currPos, end, ')');
        }
        else
            (*currPos)++;
    }
}

static void push_argv_in_list(char **comand, t_token *newToken) {
    char **meny_comand = 0;
    int id = 0;

    meny_comand = mx_strsplit(*comand, '\x0d');
    mx_strdel(comand);
    while (meny_comand[id]) {
        mx_add_to_strarr(&newToken->value, meny_comand[id++]);
        newToken->type = 1;
        newToken->priority = 10;
    }
    if (meny_comand)
        free(meny_comand);
    meny_comand = 0;
}

t_token *mx_token_in_program(int *currPos, int end, char *str,
                                 t_info *processes) {
    int tokenStart = *currPos;
    t_token *newToken = mx_create_token(2,0,0);
    char *newValue = 0;
    char *temp = 0;

    while (mx_is_char(str[tokenStart])) {
        end_argv(currPos, end, str);
        newValue = mx_strndup(&str[tokenStart], *currPos - tokenStart);
        temp = mx_audit_str(newValue, processes, 0);
        mx_strdel(&newValue);
        mx_skip_spaces(str, currPos, end);
        tokenStart = *currPos;
        push_argv_in_list(&temp, newToken);
    }
    return newToken;
}
