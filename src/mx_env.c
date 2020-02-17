#include "ush.h"

static t_var *new_node(t_list *var_tree, t_var *var) {
    var->next = malloc(sizeof(t_var));
    var = var->next;
    var->flag = 1;
    var->value = strdup(((t_variable*)var_tree->data)->mem);
    var->name = mx_strdup(((t_variable*)var_tree->data)->name);
    var->next = NULL;
    return var;
}

static t_var *var_tree_to_var(t_list *var_tree) {
    t_var *var = malloc(sizeof(t_var));
    t_var *save = var;

    var->next = NULL;
    var->value = NULL;
    for (;var_tree; var_tree = var_tree->next) {
        if (var->value == NULL && ((t_variable*)var_tree->data)->is_env) {
            var->value = strdup(((t_variable*)var_tree->data)->mem);
            var->name = mx_strdup(((t_variable*)var_tree->data)->name);
            var->flag = 1;
        }
        else if (((t_variable*)var_tree->data)->is_env)
            var = new_node(var_tree, var);
    }
    return save;
}

static void push_back(t_var *var, char *str) {
    char *name = strndup(str, mx_get_char_index(str, '='));

    while (var->next) {
        if (strcmp(var->name, name) == 0) {
            free(var->value);
            var->value = strdup(str);
            free(name);
            var->flag = 1;
            return;
        }
        var = var->next;
    }
    var->next = malloc(sizeof(t_var));
    var = var->next;
    var->value = strdup(str);
    var->flag = 1;
    var->name = name;
    var->next = NULL;
}

static bool print_env(t_var *var, char **argv, int *i) {
    if (argv[*i] == NULL) {
        mx_print_env(var);
        return 1;
    }
    for (; argv[*i]; (*i)++) {
        if (mx_reg(argv[*i], MX_REG_VER))
            push_back(var, argv[*i]);
        else
            break;
    }
    if (argv[*i] == NULL) {
        mx_print_env(var);
        return 1;
    }
    return 0;
}

void mx_env(char **argv, t_info *info) {
    t_var *var = var_tree_to_var(info->var_tree);
    char *path = NULL;
    int i = 0;

    for (i = 1; argv[i] != NULL; i++) {
        if (mx_reg(argv[i], MX_REG_VER) || mx_reg(argv[i], MX_REG_PROG))
            break;
        if (mx_check_env(argv, &path, var, &i)) {
            mx_fre_env_path(var, path);
            info->lastStatus = 1;
            return;
        }
    }
    if (print_env(var, argv, &i)) {
        mx_fre_env_path(var, path);
        return;
    }
    mx_exec_env_pr(path, mx_call_vlad(argv, i), mx_env_to_vlad(var), info);
    mx_fre_env_path(var, path);
}

