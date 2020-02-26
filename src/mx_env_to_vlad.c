#include "ush.h"

char **mx_env_to_vlad(t_var *var) {
    t_var *save = var;
    int counter = 0;
    char **env = NULL;

    if (var == NULL)
        return NULL;
    for ( ; var; var = var->next) {
        if (var->flag)
            counter++;
    }
    var = save;
    env = malloc(sizeof(char *) * (counter + 1));
    env[counter] = NULL;
    for (counter = 0; var; var = var->next) {
        if (var->flag) {
            env[counter] = mx_strdup(var->value);
            counter++;
        }
    }
    return env;
}
