#include "ush.h"

char mx_get_token_type(char *str) {
    if (str == 0)
        return 0;
    if (mx_is_char(str[0]))
        return TYPE_COMMAND;
    return TYPE_OPERATOR;
}
