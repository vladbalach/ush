#include "ush.h"

static void print_err_msg(char *str) {
    mx_printerr("u$h: Error near \'");
    mx_printerr(str);
    mx_printerr("\'\n");
}

bool mx_syntax_analyzer(t_list *tokens) {
    t_list *tmp = tokens;
    bool op = 0;

    if (tokens == 0)
        return false;
    if (((t_token*)tmp->data)->type ==  TYPE_OPERATOR) {
        print_err_msg(((t_token*)tmp->data)->value[0]);
        return false;
    }
    while(tmp->next) {
        if (((t_token*)tmp->data)->type == TYPE_OPERATOR) {
            if (op) {
                print_err_msg(((t_token*)tmp->data)->value[0]);
                return false;
            }
            op = true;
        }
        else
            op = false;
        if ((tmp->next == 0) && (op)) {
            print_err_msg(((t_token*)tmp->data)->value[0]);
            return false;
        }
        tmp = tmp->next;
    }
    if (((t_token*)tmp->data)->type ==  TYPE_OPERATOR) {
        print_err_msg(((t_token*)tmp->data)->value[0]);
        return false;
    }
    return true;
}
