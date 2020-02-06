#include "ush.h"

/* 
* -1 - not flagline
*/
int get_flags_from_line(char *str) {
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
        if (curr == -1) {
            // (*i)--;
            return 0;
        }
        flags |= curr;
    }
    i--;
    return flags;
}

static bool is_link(char *file) {
    struct stat st;
    if(lstat(file, &st) == -1) {
        // printf("stat error: %s\n", strerror(errno));
        return false;
    }
    if ((st.st_mode & S_IFLNK) == S_IFLNK) {
        // printf("LINK\n");
        return true;
    }
    return false;
}


static char* join_pwd(char *pwd, char *str) {
    int start = 0;
    int len = mx_strlen(str);
    char *buf = 0;
    if (str[0] == '/')
        start = 1;
    if (str[len - 1] == '/') {
        return mx_strjoin(pwd, &str[start]);
    }
    else {
        buf = mx_strjoin(pwd, "/");
        return mx_strjoin2(buf, &str[start]);
    }
}

bool mx_chdir(char* path, t_info *info, bool isP) {
    char *cwd = 0;
    bool isLink = is_link(path);
    
    if (chdir(path) == -1) {
        mx_printerr("ush: ");
        mx_printerr(strerror(errno));
        mx_printerr("\n");
        return false;
    }
    free(info->old_pwd);
    info->old_pwd = mx_strdup(info->pwd);
    
    if((cwd = getcwd(NULL, 0)) == NULL) {
        mx_printerr(strerror(errno));
        return false;
    }
    free(info->pwdP);
    info->pwdP = mx_strdup(cwd);
    free(info->pwdL);
    if (isLink)
        info->pwdL = join_pwd(info->old_pwd, path);
    else
        info->pwdL = mx_strdup(cwd);
    free(info->pwd);
    if(isP) 
        info->pwd = mx_strdup(info->pwdP);
    else 
        info->pwd = mx_strdup(info->pwdL);
    free(cwd);
    return true;
}

static bool is_home_or_error(char **argv, int flags, t_info *info) {
    bool isP = true ? (flags & 2) : false;
    if ((argv[0] == 0) && (!(flags & 4))) {
        mx_chdir(info->home, info, isP);
        return true;
    }
    if ((argv[0] != 0) && ((flags & 4) || (argv[1] != 0)))
        return true;
    if ((argv[0] == 0) && (flags & 4) && info->old_pwd != 0) {
            mx_chdir(info->old_pwd, info, isP);
        return true;
    }
    return false;
}



int mx_cd(char **argv, t_info *info) {
    int i = 0;
    int flags = get_flags(argv, &i);
    bool isP = true ? (flags & 2) : false;
    // char *naem =0;
    // get_path(argv[i], &naem);
    // printf("flags = %d\n", flags);
    // printf("i = %d\n", i);
    if (is_home_or_error(&argv[i], flags, info))
        return 1;
    if (is_link(argv[i]) && (flags & 1)) {
        // fprintf(stderr, "cd: not a directory: %s\n", argv[i]);
        return 1;
    }
    // printf("1\n");
    mx_chdir(argv[i], info, isP);
    // printf("pwd = %s\noldpwd = %s\n", info->pwd, info->old_pwd);
    // while(++i) {

    // }
    return 0;
}
