#include "ush.h"

void mx_execute_proces(t_token* token) {
    char **argv = 0;
    int i = 0;

    while (token->value[i]) {
        mx_add_to_strarr(&argv, token->value[i]);
        i++;
    }
    mx_add_to_strarr(&argv, token->value[i]);
    if (execvp(argv[0], argv) == -1) {
        mx_printerr("u$h: command not found: ");
        mx_printerr(token->value[0]);
        mx_printerr("\n");
        mx_del_strarr(&(token->value));
        exit(1);
    }
}
