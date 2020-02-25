#include "ush.h"

static void clearenv(t_info *info) {
    t_list *tmp = info->var_tree;

    while (tmp) {
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

static void print_error(char *err) {
    mx_printerr("u$h: ");
    mx_printerr(err);
    mx_printerr(": ");
    mx_printerr(strerror(errno));
    mx_printerr("\n");
    if (errno == 2) {
        errno = 0;
        exit(127);
    }
    else {
        errno = 0;
        exit(126); 
    }
}

static int start_child(char *path, char **argv, char **env) {
    fill_env(env);
    if (path == 0) {
        if (execvp(argv[0], argv) == -1)
            print_error(argv[0]);
    }
    else {
        char *path_buff = mx_strjoin(path, "/");
        path_buff = mx_strjoin2(path_buff, argv[0]);
        if (execv(path_buff, argv) == -1)
            print_error(argv[0]);
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
