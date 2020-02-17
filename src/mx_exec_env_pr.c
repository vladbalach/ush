#include "ush.h"

static void clearenv(t_info *info) {
    t_list *tmp = info->var_tree;

    while(tmp) {
        if (((t_variable*)tmp->data)->is_env)
            unsetenv(((t_variable*)tmp->data)->name);
        tmp = tmp->next;
    }
}

static void fill_env(char **env) {
    int i = 0;

    while (env[i]) {
        putenv(env[i]);
        i++;
    }
}

static int start_child(char *path, char **argv, char **env) {
    fill_env(env);
    if (path == 0) {
        if (execvp(argv[0], argv) == -1) {
            mx_printerr("u$h: command not found: ");
            mx_printerr(argv[0]);
            mx_printerr("\n");
            exit(1);
        }
    }
    else {
        if (execv(path, argv) == -1) {
            mx_printerr("u$h: command not found: ");
            mx_printerr(path);
            mx_printerr("\n");
            exit(1);
        }
        
    }
    return 0;
}

void mx_exec_env_pr(char *path, char **argv, char **env, t_info *info) {
    pid_t pid = fork();

    if (pid == 0) {
        clearenv(info);
        start_child(path, argv, env);
    }
    else {
        mx_wait_process(info, argv);
    }
    mx_del_strarr(&env);
    mx_del_strarr(&argv);
}