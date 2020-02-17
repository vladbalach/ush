#include "ush.h"

int mx_chdir_P(char *path, t_info *info, char flags) {
    if(chdir(path) == -1) {
        if ((flags & 1) == 0)
            fprintf(stderr, "cd: %s: %s\n", strerror(errno), path);
        return 1;
    }
    free(info->old_pwd);
    info->old_pwd = info->pwd;
    info->pwd = getcwd(NULL,0);
    free(info->pwdL);
    info->pwdL = getcwd(NULL,0);
    free(info->pwdP);
    info->pwdP = getcwd(NULL,0);
    free(info->pwd);
    info->pwd = getcwd(NULL,0);
    return 0;
}
