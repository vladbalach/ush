#include "ush.h"

void mx_fre_env_path(t_var *var, char *path) {
    t_var *tmp = NULL;

    if (path) {
        free(path);
        path = NULL;
    }
    while (var) {
        tmp = var;
        var = var->next;
        free(tmp->value);
        free(tmp->name);
        free(tmp);
    }
}
