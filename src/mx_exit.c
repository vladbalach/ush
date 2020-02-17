#include "ush.h"

void mx_exit(t_token *token, t_info *info) {
    int s = 0;

    if (token->value[1]) {
        s = atoi(token->value[1]);
        info->exit_status = s;
    }
    else
        info->exit_status = 0;
    info->isExit = true;
}
