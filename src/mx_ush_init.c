#include "ush.h"

void mx_ush_init(t_info **info, char **env) {
    t_info *newInfo = (t_info*) malloc(sizeof(t_info));

    newInfo->env = env;
    newInfo->processes = 0; // empty
    newInfo->isExit = false;
    newInfo->exit_status = 0;
    newInfo->pwd = getcwd(NULL, 0);
    newInfo->pwdP = getcwd(NULL, 0);
    newInfo->pwdL = getcwd(NULL, 0);
    newInfo->old_pwd = getcwd(NULL, 0);
    newInfo->lastStatus = 0;
    (*info) = newInfo;
    mx_start_program(&(newInfo->var_tree), env);
    signal(SIGSEGV, mx_segfault);
    signal(SIGINT, mx_ctrl_c);
    signal(SIGTSTP, mx_ctrl_z);
}
