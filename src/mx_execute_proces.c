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

void mx_execute_proces(t_token* token) {
    char **argv = 0;
    int i = 0;

    for (; token->value[i]; i++)
        mx_add_to_strarr(&argv, token->value[i]);
    mx_add_to_strarr(&argv, token->value[i]);
    if (execvp(argv[0], argv) == -1) {
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
}



