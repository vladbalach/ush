#include "ush.h"

static char* mx_get_link_path(char *path) {
    char *link_path = malloc(1024);
    int len = 0;

    if ((len = readlink(path, link_path, sizeof(link_path) - 1)) == -1) {
        return 0;
    }
    link_path[len] = 0;
    return link_path;
}

static char* get_pwd() {
    char *pwd = mx_strdup(getenv("PWD"));
    char *cwd = getcwd(NULL, 0);
    char *link_path = 0;

    if (pwd == 0)
        return cwd;
    if (mx_is_link(pwd)) {
        link_path = mx_get_link_path(pwd);
        pwd = mx_del_last_rank(pwd);
        pwd = mx_strjoin3(mx_strjoin2(pwd, "/"), link_path);
        if (mx_strcmp(pwd, cwd) == 0) {
            free(cwd);
            free(pwd);
            return mx_strdup(getenv("PWD"));
        }
    }
    free(pwd);
    return cwd;
}

void mx_ush_init(t_info **info, char **env) {
    t_info *newInfo = (t_info*) malloc(sizeof(t_info));

    newInfo->env = env;
    newInfo->processes = 0; // empty
    newInfo->isExit = false;
    newInfo->exit_status = 0;
    newInfo->pwd = get_pwd();
    newInfo->pwdP = getcwd(NULL, 0);
    newInfo->pwdL = mx_strdup(newInfo->pwd);
    newInfo->old_pwd = get_pwd();
    newInfo->lastStatus = 0;
    (*info) = newInfo;
    mx_start_program(&(newInfo->var_tree), env);
    signal(SIGSEGV, mx_segfault);
    signal(SIGINT, mx_ctrl_c);
    signal(SIGTSTP, mx_ctrl_z);
}
