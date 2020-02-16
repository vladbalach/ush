#include "ush.h"

static void change_variteble(t_variable *noda, char *name, char *value) {
    char *temp = 0;

    mx_strdel(&((t_variable *)noda)->name);
    mx_strdel(&((t_variable *)noda)->value);
    noda->name = name;
    noda->value = value;
    if (noda->is_env) {
        temp = mx_strjoin(name, "=");
        temp = mx_strjoin2(temp, value);
        mx_strdel(&(noda->mem));
        noda->mem = temp;
        putenv(temp);
    }
}

void mx_serch_list(t_list **var_tree, char *name, char *value) {
    t_list *var_tree_temp = *var_tree;
    t_variable *var = 0;
    int i = 1;

    while (i == 1 && var_tree_temp) {
        if (mx_strcmp(((t_variable*)var_tree_temp->data)->name, name) == 0) {
            change_variteble(var_tree_temp->data, name, value);
            i = 0;
        }
        else
            var_tree_temp = var_tree_temp->next;
    }
    if (!(var_tree_temp)) {
        var = (t_variable*) malloc(sizeof(t_variable));
        var->mem = 0;
        var->is_env = false;
        var->name = name;
        var->value = value;
        mx_push_back(var_tree, var);
    }
}
