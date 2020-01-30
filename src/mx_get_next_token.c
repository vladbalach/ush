#include "ush.h"

static void set_current_pos(int *currPos, char c, char *str) {
    if (str[*currPos] == c) {
        *currPos += 1;
        if (str[*currPos] == c) {
            *currPos += 1;
        }
    }
}

static bool is_operator(char c) {
    if (c == '|' || c == '&' || c == '>' || c == '<')
        return true;
    return false;
}

static void mx_skip_spaces (char *str, int *start, int end) {
    while (str[*start] && *start < end) {
        if (str[*start] != ' ') {
            return;
        }
        (*start)++;
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


t_token* mx_get_next_token(int *currPos, int end, char *str) {
    int tokenStart = *currPos;
    // int tokenEnd = *currPos;
    t_token *newToken = 0;
    char *newValue = 0;

    if (*currPos >= end)
        return 0;
    newToken = mx_create_token(0,0,0);
    if (mx_is_char(str[tokenStart])) {
        while (mx_is_char(str[tokenStart])) {
            while (mx_is_char(str[*currPos])) {
                (*currPos)++;
            }
            newValue = mx_strndup(&str[tokenStart], *currPos - tokenStart);
            mx_skip_spaces(str, currPos, end);
            tokenStart = *currPos;
            mx_add_to_strarr(&newToken->value, newValue);
        }
    }

    else if (is_operator(str[tokenStart])) {
        set_current_pos(currPos, str[tokenStart], str);
        newValue = mx_strndup(&str[tokenStart], *currPos - tokenStart);
        mx_add_to_strarr(&newToken->value, newValue);
    }
    newToken->type = mx_get_token_type(newToken->value[0]);
    newToken->priority = get_token_priority(newToken->value[0]);
    mx_skip_spaces(str, currPos, end);
    return newToken;
}

// t_token* mx_get_next_token(int *currPos, int end, char *str) {
//     int tokenStart = *currPos;
//     int tokenEnd = *currPos;
//     t_token *newToken = 0;

//     if (*currPos >= end)
//         return 0;
//     newToken = (t_token*) malloc (sizeof(t_token));
//     if (mx_is_char(str[tokenStart])) {
//         while (mx_is_char(str[*currPos])) {
//             (*currPos)++;
//         }
//     }
//     else if (is_operator(str[tokenStart])) {
//         set_current_pos(currPos, str[tokenStart], str);
//     }
//     newToken->value = mx_strndup(&str[tokenStart], *currPos - tokenStart);
//     newToken->type = mx_get_token_type(newToken->value);
//     newToken->priority = get_token_priority(newToken->value);
//     mx_skip_spaces(str, currPos, end);
    
//     return newToken;
// }