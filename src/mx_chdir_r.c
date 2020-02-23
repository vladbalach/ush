#include "ush.h"

int mx_chdir_r(char *path, t_info *info, char flags) {
    if (path == 0)
        return 0;
    if(chdir(path) == -1) {
        if ((flags & 1) == 0)
            fprintf(stderr, "cd: %s: %s\n", strerror(errno), path);
        return 1;
    }
    free(info->old_pwd);
    info->old_pwd = info->pwd;
    info->pwd = getcwd(NULL, 0);
    free(info->pwd_l);
    info->pwd_l = getcwd(NULL, 0);
    free(info->pwd_p);
    info->pwd_p = getcwd(NULL, 0);
    free(info->pwd);
    info->pwd = getcwd(NULL, 0);
    return 0;
}
