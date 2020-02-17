#include "ush.h"

static void mx_skip_spaces (char *str, int *start, int end) {
    while (str[*start] && *start < end) {
        if (str[*start] != ' ') {
            return;
        }
        (*start)++;
    }
}

static void set_current_pos(int *currPos, char c, char *str) {
    if (str[*currPos] == c) {
        *currPos += 1;
        if (str[*currPos] == c) {
            *currPos += 1;
        }
    }
}

static int get_token_priority(char *str) {
    if (mx_strcmp(str, "||") == 0 || mx_strcmp(str, "&&") == 0)
        return 60;
    else if (str[0] == '|')
        return 50;
    else if (str[0] == '>')
        return 40;
    else if (str[0] == '<')
        return 30;
    else if (str[0] == '&')
        return 20;
    else
        return 10;
}

static bool is_operator(char c) {
    if (c == '|' || c == '&' || c == '>' || c == '<')
        return true;
    return false;
}

t_token* mx_get_next_token(int *currPos, int end, char *str,
                           t_info *processes) {
    int tokenStart = *currPos;
    t_token *newToken = 0;
    char *newValue = 0;

    if (*currPos >= end)
        return 0;
    if (mx_is_char(str[tokenStart]))
        newToken = mx_token_in_program(currPos, end, str, processes);
    else if (is_operator(str[tokenStart])) {
        newToken = mx_create_token(2,0,0);
        set_current_pos(currPos, str[tokenStart], str);
        newValue = mx_strndup(&str[tokenStart], *currPos - tokenStart);
        mx_add_to_strarr(&newToken->value, newValue);
        newToken->type = mx_get_token_type(newToken->value[0]);
        newToken->priority = get_token_priority(newToken->value[0]);
    }
    mx_skip_spaces(str, currPos, end);
    return newToken;
}
