#include "ush.h"

static void print_err(t_token *token) {
    if (errno == 2)
        mx_printerr("u$h: command not found: ");
    else {
        mx_printerr("u$h: ");
        mx_printerr(strerror(errno));
        mx_printerr(": ");
    }
    mx_printerr(token->value[0]);
    mx_printerr("\n");
}

static void exec(t_token* token) {
    print_err(token);
    mx_del_strarr(&(token->value));
    if (errno == 2) {
        errno = 0;
        exit(127);
    }
    else {
        errno = 0;
        exit(126);
    }
}

void mx_execute_proces(t_token* token) {
    if (getenv("PATH") == 0)
        if (execv(token->value[0], token->value) == -1)
            exec(token);
    if (execvp(token->value[0], token->value) == -1)
        exec(token);
}



