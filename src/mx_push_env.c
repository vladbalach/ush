#include "ush.h"

void mx_push_env(t_list **var_tree, char *name, char *value, char *mem) {
    t_variable *var = (t_variable*) malloc(sizeof(t_variable));

    var->value = value;
    var->is_env = true;
    var->name = name;
    var->mem = mem;
    putenv(var->mem);
    mx_push_front(var_tree, var);
}