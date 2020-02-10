#include "ush.h"

static void check_parametr(t_list **var_tree, char *name, char *value, char *mem) {
    t_list *var_tree_temp = *var_tree;
    t_variable *var = 0;
    char *temp = 0;
    int check = 1;

    while (check == 1 && var_tree_temp) {
        if (mx_strcmp(((t_variable*)var_tree_temp->data)->name, name) == 0) {
            if (((t_variable*)var_tree_temp->data)->is_env == false) {
                ((t_variable*)var_tree_temp->data)->is_env = true;
                if (!(mem)) {
                    temp = mx_strjoin(((t_variable*)var_tree_temp->data)->name, "=");
                    temp = mx_strjoin2(temp, ((t_variable*)var_tree_temp->data)->value);
                    ((t_variable*)var_tree_temp->data)->mem = temp;
                }
                else {
                    mx_strdel(&(((t_variable*)var_tree_temp->data)->value));
                    ((t_variable*)var_tree_temp->data)->value = value;
                    temp = mx_strdup(mem);
                }
                putenv(temp);
            }
            else {
                if (mem) {
                    mx_strdel(&(((t_variable*)var_tree_temp->data)->value));
                    ((t_variable*)var_tree_temp->data)->value = value;
                    mx_strdel(&(((t_variable*)var_tree_temp->data)->mem));
                    ((t_variable*)var_tree_temp->data)->mem = mx_strdup(mem);;
                    putenv(((t_variable*)var_tree_temp->data)->mem);
                }
            }
            check = 0;
        }
        else
            var_tree_temp = var_tree_temp->next;
    }
    if (check == 1 && mem) {
        var = (t_variable*) malloc(sizeof(t_variable));
        var->name = mx_strdup(name);
        var->value = value;
        var->mem = mx_strdup(mem);
        var->is_env = true;
        putenv(var->mem);
        mx_push_back(var_tree, var);
    }
}


void mx_export(char **argv, t_list **var_tree) {
    char *temp = 0;
    char **parametr = 0;

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
