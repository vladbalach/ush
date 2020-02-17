#include "ush.h"


static void not_env(t_variable *var, char *name, char *value, char *mem) {
    char *temp = NULL;

    var->is_env = true;
    if (!(mem)) {
        temp = mx_strjoin(name, "=");
        temp = mx_strjoin2(temp, var->value);
        var->mem = temp;
    }
    else {
        mx_strdel(&(var->value));
        var->value = value;
        temp = mx_strdup(mem);
    }
    putenv(temp);
}

static void have_list_name(t_variable *var, char *name, char *value,
                           char *mem) {
    if (var->is_env == false)
        not_env(var, name, value, mem);
    else {
        if (mem) {
            mx_strdel(&(var->value));
            var->value = value;
            mx_strdel(&(var->mem));
            var->mem = mx_strdup(mem);
            putenv(var->mem);
        }
    }
}

static void check_parametr(t_list **var_tree, char *name, char *value,
                           char *mem) {
    t_list *var_tree_temp = *var_tree;
    t_variable *var = 0;
    int check = 1;

    while (check == 1 && var_tree_temp) {
        if (mx_strcmp(((t_variable *)var_tree_temp->data)->name, name) == 0) {
            have_list_name(var_tree_temp->data, name, value, mem);
            check = 0;
        }
        var_tree_temp = var_tree_temp->next;
    }
    if (check == 1 && mem) {
        var = (t_variable *) malloc(sizeof(t_variable));
        var->name = mx_strdup(name);
        var->value = value;
        var->mem = mx_strdup(mem);
        var->is_env = true;
        putenv(var->mem);
        mx_push_back(var_tree, var);
    }
}


void mx_export(char **argv, t_list **var_tree, t_info *info) {
    char **parametr = 0;

    info->lastStatus = 0;
    for (int i = 1; argv[i]; i++) {
        parametr = mx_strsplit(argv[i], '=');
        if (!(parametr[1])) {
            check_parametr(var_tree, argv[i], 0, 0);
            mx_del_strarr(&parametr);
        }
        else if (!(parametr[2])) {
            check_parametr(var_tree, parametr[0], parametr[1], argv[i]);
            mx_strdel(&parametr[0]);
            free(parametr);
        }
        else
            mx_del_strarr(&parametr);
    }
}
