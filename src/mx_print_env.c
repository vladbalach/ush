#include "ush.h"

void mx_print_env(t_var *var) { 
    while (var) {
        if (var->flag) {
            printf("%s\n", var->value);
        }
        var = var->next;
    }
}
