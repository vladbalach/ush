#include "ush.h"


static void not_env(t_variable *var, char *name, char *value) {
    char *temp = NULL;

    var->is_env = true;
    if (value) {
        mx_strdel(&(var->value));
        var->value = mx_strdup(value); 
    }
    temp = mx_strjoin(name, "=");
    temp = mx_strjoin2(temp, var->value);
    var->mem = temp;
    putenv(var->mem);
}

static void have_list_name(t_variable *var, char *name, char *value) {
    char *temp = NULL;

    if (var->is_env == false)
        not_env(var, name, value);
    else {
        if (value) {
            mx_strdel(&(var->value));
            var->value = mx_strdup(value);
            mx_strdel(&(var->mem));
            temp = mx_strjoin(name, "=");
            temp = mx_strjoin2(temp, var->value);
            var->mem = temp;
            putenv(var->mem);
        }
    }
}

static void check_parametr(t_list **var_tree, char *name, char *value) {
    t_list *var_tree_temp = *var_tree;
    t_variable *var = 0;
    int check = 1;

    while (check == 1 && var_tree_temp) {
        if (name  && mx_strcmp(((t_variable *)var_tree_temp->data)->name,
                               name) == 0) {
            have_list_name(var_tree_temp->data, name, value);
            check = 0;
        }
        var_tree_temp = var_tree_temp->next;
    }
    if (check == 1 && value) {
        var = (t_variable *) malloc(sizeof(t_variable));
        var->name = mx_strdup(name);
        var->value = 0;
        not_env(var, name, value);
        mx_push_back(var_tree, var);
    }
}

static void print_env(t_list **var_tree) {
    t_list *var_tree_temp = *var_tree;

    while (var_tree_temp) {
        if (((t_variable *)var_tree_temp->data)->is_env) {
            mx_printstr(((t_variable *)var_tree_temp->data)->mem);
            mx_printstr("\n");
        }
        var_tree_temp = var_tree_temp->next;
    }
}

void mx_export(char **argv, t_list **var_tree, t_info *info) {
    char **parametr = 0;
    char *temp = 0;

    info->lastStatus = 0;
    for (int i = 1; argv[i]; i++) {
        parametr = mx_strsplit(argv[i], '=');
        if (parametr[0] && argv[i][mx_strlen(parametr[0])] == '=')
            temp = mx_strdup(&(argv[i][mx_strlen(parametr[0]) + 1]));
        if (parametr[1] == 0 || parametr[2] == 0)
            check_parametr(var_tree, parametr[0], temp);
        mx_del_strarr(&parametr);
        mx_strdel(&temp);
    }
    if (argv[1] == 0)
        print_env(var_tree);
}
