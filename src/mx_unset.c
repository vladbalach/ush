#include "ush.h"

static bool check_argv(char *str) {
    for (int i = 0 ; str[i]; i++)
        if (str[i] == '=') {
            mx_printerr("unset: ");
            mx_printerr(str);
            mx_printerr(": invalid parameter name\n");
            return 0;
        }
    return 1;
}

static void del_t_variable(void *str) {
    t_variable *var= str;

    mx_strdel(&(var->name));
    mx_strdel(&(var->value));
    if (var->is_env)
        mx_strdel(&(var->mem));
}

static bool if_argv(void *s1, void *s2) {
    char *temp1 = s1;
    char *temp2 = ((t_variable *)s2)->name;

    if (mx_strcmp(temp1, temp2) == 0)
        return 1;
    else
        return 0;
}

void mx_unset(char **argv, t_list **var_tree) {
    for (int i = 1; argv[i]; i++) {
        if (check_argv(argv[i])) {
            mx_pop_list(var_tree, argv[i], if_argv, del_t_variable);
        }
    }
}
