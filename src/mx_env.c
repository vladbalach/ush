#include "ush.h"

void mx_env(char **argv, t_list *var_tree) {
    t_list *var_tree_temp = var_tree;

    while (var_tree_temp) {
        if (((t_variable*)var_tree_temp->data)->is_env) {
            mx_printstr(((t_variable*)var_tree_temp->data)->mem);
            write(1,"\n",1);
        }
        var_tree_temp = var_tree_temp->next;
    }
}
