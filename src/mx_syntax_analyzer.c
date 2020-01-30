#include "ush.h"

static void print_err_msg(char *str) {
    mx_printerr("\033[1;31mu$h: Error near \'");
    mx_printerr(str);
    mx_printerr("\'\033[0m\n");
}

static bool is_operator(t_token *token) {
    if (token->type ==  TYPE_OPERATOR) {
        print_err_msg(token->value[0]);
        return true;
    }
    return false;
}
static bool is_double_op(t_list *tmp, bool *op) {
    if (((t_token*)tmp->data)->type == TYPE_OPERATOR) {
        if (*op) {
            print_err_msg(((t_token*)tmp->data)->value[0]);
            return true;
        }
        *op = true;
    }
    else
        *op = false;
    if ((tmp->next == 0) && (*op)) {
        print_err_msg(((t_token*)tmp->data)->value[0]);
        return false;
    }
    return false;
}

static bool is_double_more(t_list *tmp) {
    char *value = ((t_token*)tmp->data)->value[0];
    if ((mx_strcmp(value, ">") == 0) || (mx_strcmp(value, ">>") == 0)) {
        if ((tmp->next) && (tmp->next->next) 
            && ((mx_strcmp(((t_token*)tmp->next->next->data)->value[0], ">") == 0
            || mx_strcmp(((t_token*)tmp->next->next->data)->value[0], ">>") == 0
            || mx_strcmp(((t_token*)tmp->next->next->data)->value[0], "<") == 0))) {
                print_err_msg(value);
                return true;
        }
    }
    return false;
}

static bool is_double_less(t_list *tmp) {
    char *value = ((t_token*)tmp->data)->value[0];
    if (mx_strcmp(value, "<") == 0) {
        if ((tmp->next) && (tmp->next->next) 
            && (mx_strcmp(((t_token*)tmp->next->next->data)->value[0], "<") == 0)) {
                print_err_msg(value);
                return true;
        }
    }
    return false;
}

static bool mx_is_ampersand(t_list *tmp) {
    if (mx_strcmp(((t_token*)tmp->data)->value[0], "&") == 0) {
        mx_printerr("\033[1;31m\'&\' - not allowed in this version!\033[0m\n");
        return true;
    }
    return false;
}

bool mx_syntax_analyzer(t_list *tokens) {
    t_list *tmp = tokens;
    bool op = 0;

    if (tokens == 0)
        return false;
    if (is_operator((t_token*)tmp->data))
        return false;
    while(tmp->next) {
        if (is_double_op(tmp, &op))
            return false;
        if (is_double_more(tmp))
            return false;
        if (is_double_less(tmp))
            return false;
        if (mx_is_ampersand(tmp)) {
            return false;
        }
        tmp = tmp->next;
    }
    if (is_operator((t_token*)tmp->data))
        return false;
    return true;
}
