#include "ush.h"

bool mx_is_operator(t_token *token) {
    if ((token->type ==  TYPE_OPERATOR) && (token->priority != 20)) {
        mx_printerr("\033[1;31mu$h: Error near \'");
        mx_printerr(token->value[0]);
        mx_printerr("\'\033[0m\n");
        return true;
    }
    return false;
}
