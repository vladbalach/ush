#include "ush.h"

/* 
* -1 - not flagline
*/
static int get_flags_from_line(char *str) {
    int i = 0;
    int flags = 0;

    if (str[i + 1] == '\0') 
        return 4; // - flag
    while(str[++i]) {
        if (str[i] == 's') 
            flags |= 1;
        else if (str[i] == 'P') 
            flags |= 2;
        else {
            return -1;
        }
    }
    return flags;
}

/* 
* 1-st bit -> -s
* 2-nd bit -> -P
* 3-rd bit -> -
*/
static int get_flags(char **argv, int *i) {
    int flags = 0;

    while(argv[++(*i)]) {
        if (argv[(*i)][0] != '-')
            return flags;
        int curr = get_flags_from_line(argv[*i]);
        if (curr == -1)
            return 0;
        flags |= curr;
    }
    i--;
    return flags;
}

static void export_pwd_oldpwd(t_info *info) {
    char **temp = (char **) malloc(4 * sizeof (char *));

    temp[0] = mx_strdup("cd");
    temp[1] = mx_strjoin("PWD=", info->pwd);
    temp[2] = mx_strjoin("OLDPWD=", info->old_pwd);
    temp[3] = 0;
    mx_export(temp, &info->var_tree);
    mx_del_strarr(&temp);
}

int mx_cd(char **argv, t_info *info) {
    int i = 0;
    int flags = get_flags(argv, &i);
    char *path = (flags & 4) ? info->old_pwd : MX_GET_PATH;
    int status  = 0;

    if (mx_is_link(path) && (flags & 1) && (flags & 2) == 0) {
        fprintf(stderr, "cd: not a directory: %s\n", argv[i]);
        return 1;
    }
    if (flags & 2)
        status = mx_chdir_P(path, info, flags);
    else
        status = mx_chdir_L(path, info, flags);
    if (status == 0)
        export_pwd_oldpwd(info);
    info->lastStatus = status;
    return status;
}

