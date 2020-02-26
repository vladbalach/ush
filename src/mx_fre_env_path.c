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
        if (tmp->value)
            free(tmp->value);
        if (tmp->name)
            free(tmp->name);
        free(tmp);
    }
}
