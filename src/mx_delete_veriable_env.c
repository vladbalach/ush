#include "ush.h"

static void chenge_var(t_var *var, char *name) {
    while (var) {
        if (strcmp(name, var->name) == 0) {
            var->flag = 0;
            break;
        }
        var = var->next;
    }
}

int mx_delete_veriable_env(char *str1, char *str2, t_var *var, int *i) {
    char *name = NULL;

    if (mx_strlen(str1) > 2) {
        str1 += 2;
        name = mx_strdup(str1);
    }
    else {
        (*i)++;
        if (str2)
            name = mx_strdup(str2);
    }
    if (name == NULL)
        return 1;
    chenge_var(var, name);
    free(name);
    return 0;
}
