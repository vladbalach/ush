#include "ush.h"

void mx_clear_tokens(t_list **tokens) {
    while(*tokens) {
        t_token *tok = (*tokens)->data;
        free(tok->value);
        mx_pop_front(tokens);
    }
}
