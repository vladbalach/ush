#include "ush.h"

void mx_clear_tokens(t_list **tokens) {
    t_token *tok = 0;

    while(*tokens) {
        tok = (*tokens)->data;
        mx_del_strarr(&tok->value);
        mx_pop_front(tokens);
    }
}
