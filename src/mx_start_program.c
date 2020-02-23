#include "ush.h"

static void shlvl_new(char **s) {
    int a = 0;
    char *temp = 0;
    int i = 0;
    int neg = 1;

    if (s[0][0] == '-') {
        i++;
        neg = -1;
    }
    for (; i < mx_strlen(s[0]); i++){
        a = a * 10 + (s[0][i] - '0');
    }
    a = a * neg;
    mx_strdel(s);
    temp = mx_itoa(++a);
    *s = temp;
}

static char *create_mem(char *str1, char **str2, char *str3) {
    char *mem = 0;

    if (mx_strcmp(str1, "SHLVL") == 0) {
        shlvl_new(str2);
        mem = mx_strjoin("SHLVL=", *str2);
    }
    else if (mx_strcmp(str1, "PWD") == 0) {
        mx_strdel(str2);
        *str2 = getcwd(NULL, 0);
        mem = mx_strjoin("PWD=", *str2);
    }
    else if (mx_strcmp(str1, "OLDPWD") == 0) {
        mx_strdel(str2);
        *str2 = getcwd(NULL, 0);
        mem = mx_strjoin("OLDPWD=", *str2);
    }
    else
        mem = mx_strdup(str3);
    return mem;
}

static bool check_if_env_have(char *name, t_list **var_tree) {
    t_list *var_tree_check = *var_tree;

    while (var_tree_check) {
        if (mx_strcmp(((t_variable *)var_tree_check->data)->name, name) == 0)
            return false;
        else
            var_tree_check = var_tree_check->next;
    }
    return true;
}

static void check_env(t_list **var_tree) {
    char *temp = 0;

    if (check_if_env_have("PWD", var_tree)) {
        temp = getcwd(NULL, 0);;
        mx_push_env(var_tree, mx_strdup("PWD"),
                    temp, mx_strjoin("PWD=", temp));
    }
    if (check_if_env_have("OLDPWD", var_tree)) {
        temp = getcwd(NULL, 0);
        mx_push_env(var_tree, mx_strdup("OLDPWD"), temp,
                    mx_strjoin("OLDPWD=", temp));
    }
    if (check_if_env_have("SHLVL", var_tree)) {
        temp = mx_strdup("1");
        mx_push_env(var_tree, mx_strdup("SHLVL"), temp,
                    mx_strjoin("SHLVL=", temp));
    }
}

void mx_start_program(t_list **var_tree, char **env) {
    int i = -1;
    char **envvar = 0;
    char *temp = 0;
    t_variable *var = 0;

    while (env[++i]) {
        var = (t_variable*) malloc(sizeof(t_variable));
        envvar = mx_strsplit(env[i], '=');
        temp = create_mem(envvar[0], &envvar[1], env[i]);
        var->is_env = true;
        var->name = envvar[0];
        var->value = mx_strdup(&env[i][mx_strlen(var->name)]);
        var->mem = temp;
        putenv(temp);
        mx_push_front(var_tree, var);
        free(envvar[1]);
        free(envvar);
    }
    check_env(var_tree);
}
