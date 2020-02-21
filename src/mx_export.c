#include "ush.h"

static void new_node(t_list **var_tree, char *name, char *value, char *tmp) {
    t_variable *node = malloc(sizeof(t_variable));

    node->name = name;
    node->value = value;
    node->mem = value == 0 ? tmp : mx_strjoin2(tmp, value);
    node->is_env = true;
    putenv(node->mem);
    mx_push_back(var_tree, node);
}

static void get_parametr(t_list **var_tree, char *str) {
    int counter = mx_get_char_index(str, '=');
    char *name = counter == -1 ? mx_strdup(str) : mx_strndup(str, counter);
    char *value = counter == -1 ? 0 : mx_strdup(str + counter + 1);
    t_list *var_tree_tmp = *var_tree;
    char *tmp = mx_strjoin(name, "=");

    for (; var_tree_tmp; var_tree_tmp = var_tree_tmp->next) {
        if (strcmp(((t_variable*)(var_tree_tmp->data))->name, name) == 0) {
            free(((t_variable*)(var_tree_tmp->data))->value);
            ((t_variable*)(var_tree_tmp->data))->value = value;
            free(((t_variable*)(var_tree_tmp->data))->mem);
            ((t_variable*)(var_tree_tmp->data))->mem = value == 0 ? tmp 
                : mx_strjoin2(tmp, value);
            free(name);
            ((t_variable*)(var_tree_tmp->data))->is_env = true;
            putenv(((t_variable*)(var_tree_tmp->data))->mem);
            return;
        }
    }
    new_node(var_tree, name, value, tmp);
}

static void swap(t_var *var) {
    char *name = var->name;
    char *value = var->value;
    int flag = var->flag;

    var->name = var->next->name;
    var->value = var->next->value;
    var->flag = var->next->flag;
    var->next->name = name;
    var->next->value = value;
    var->next->flag = flag;
}

static void print_export(t_list *var_tree_tmp) {
    t_var *var = mx_var_tree_to_var(var_tree_tmp);
    t_var *tmp = var;

    for (t_var *i = var; i; i = i->next) {
        for (t_var *j = var; j->next; j = j->next) {
            if (mx_strcmp(j->name, j->next->name) > 0)
                swap(j);
        }
    }
    while (var) {
        mx_printstr(var->value);
        mx_printstr("\n");
        tmp = var;
        var = var->next;
        free(tmp->value);
        free(tmp->name);
        free(tmp);
    }
}

void mx_export(char **argv, t_list **var_tree, t_info *info) {
    info->lastStatus = 0;
    for (int i = 0; argv[i]; i++) {
        if (mx_reg(argv[i], "")) {
            mx_printerr("export: not valid in this context:");
            mx_printerr(argv[i]);
            info->lastStatus = 1;
            return;
        }
    }
    for (int i = 1; argv[i]; i++) {
        if (mx_reg(argv[i], MX_REG_EXPORT))
            get_parametr(var_tree, argv[i]);
    }
    if (argv[1] == 0)
        print_export(*var_tree);
}
