#include "ush.h"






// static char* join_pwd(char *pwd, char *str) {
//     int start = 0;
//     int len = mx_strlen(str);
//     char *buf = 0;
//     if (str[0] == '/')
//         start = 1;
//     if (str[len - 1] == '/') {
//         return mx_strjoin(pwd, &str[start]);
//     }
//     else {
//         buf = mx_strjoin(pwd, "/");
//         return mx_strjoin2(buf, &str[start]);
//     }
// }

// bool mx_chdir(char* path, t_info *info, bool isP) {
//     char *cwd = 0;
//     bool isLink = is_link(path);
    
//     if (chdir(path) == -1) {
//         mx_printerr("ush: ");
//         mx_printerr(strerror(errno));
//         mx_printerr("\n");
//         return false;
//     }
//     free(info->old_pwd);
//     info->old_pwd = mx_strdup(info->pwd);
    
//     if((cwd = getcwd(NULL, 0)) == NULL) {
//         mx_printerr(strerror(errno));
//         return false;
//     }
//     free(info->pwdP);
//     info->pwdP = mx_strdup(cwd);
//     free(info->pwdL);
//     if (isLink)
//         info->pwdL = join_pwd(info->old_pwd, path);
//     else
//         info->pwdL = mx_strdup(cwd);
//     free(info->pwd);
//     if(isP) 
//         info->pwd = mx_strdup(info->pwdP);
//     else 
//         info->pwd = mx_strdup(info->pwdL);
//     free(cwd);
//     return true;
// }

// static bool is_home_or_error(char **argv, int flags, t_info *info) {
//     bool isP = true ? (flags & 2) : false;
//     if ((argv[0] == 0) && (!(flags & 4))) {
//         mx_chdir(info->home, info, isP);
//         return true;
//     }
//     if ((argv[0] != 0) && ((flags & 4) || (argv[1] != 0)))
//         return true;
//     if ((argv[0] == 0) && (flags & 4) && info->old_pwd != 0) {
//             mx_chdir(info->old_pwd, info, isP);
//         return true;
//     }
//     return false;
// }



// int mx_cd(char **argv, t_info *info) {
//     int i = 0;
//     int flags = get_flags(argv, &i);
//     bool isP = true ? (flags & 2) : false;
//     // char *naem =0;
//     // get_path(argv[i], &naem);
//     // printf("flags = %d\n", flags);
//     // printf("i = %d\n", i);
//     if (is_home_or_error(&argv[i], flags, info))
//         return 1;
//     if (is_link(argv[i]) && (flags & 1)) {
//         // fprintf(stderr, "cd: not a directory: %s\n", argv[i]);
//         return 1;
//     }
//     // printf("1\n");
//     mx_chdir(argv[i], info, isP);
//     // printf("pwd = %s\noldpwd = %s\n", info->pwd, info->old_pwd);
//     // while(++i) {

//     // }
//     return 0;
// }

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
        if (curr == -1)
            return 0;
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

// static char* mx_get_link_path(char *link) {
//     char *linkpath = malloc(2048);
//     int len = 0;
//     if((len = readlink(link, linkpath, sizeof(linkpath)-1)) == -1) {
//         mx_printerr(strerror(errno));
//         return 0;
//     }
//     else
//         linkpath[len] = 0;
//     return linkpath;
// }

int mx_chdir_P(char *path, t_info *info) {
    if(chdir(path) == -1) {
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

static int get_rank(char *path) {
    int i = -1;
    int rank = 0;
    while(path[++i]) {
        if ((path[i] == '/') && (path[i + 1])) {
            rank++;
        }
    }
    return rank;
}

static char* mx_del_last_rank(char *path) {
    char *new_path = 0;
    int len = mx_strlen(path);
    int i = len;

    while (path[--i] && path[i] != '/')
            path[i] = 0;
    if ((path[i] == '/') && (i != 0))
        path[i] = 0;
    new_path = mx_strdup(path);
    free(path);
    return new_path;
}

static char* mx_add_one_rank(char *path, char *new_part) {
    int len_path = mx_strlen(path);
    char *new_path = 0;
    if (path[len_path - 1] != '/')
        new_path = mx_strjoin(path, "/");
    new_path = mx_strjoin2(new_path, new_part);
    free(path);
    return new_path;
}

char *get_new_pwd(char *path, t_info *info) {
    char **tokens = mx_strsplit(path, '/');
    int i = -1;
    char *tmp_pwd = path[0] == '/' ? mx_strdup("/") : mx_strdup(info->pwdL);
    char *res = 0;
    while (tokens[++i]) {
        if (mx_strcmp(tokens[i], "..") == 0) {
            if (get_rank(tmp_pwd) == 0)
                continue;
            tmp_pwd = mx_del_last_rank(tmp_pwd);
        }
        else 
            tmp_pwd = mx_add_one_rank(tmp_pwd, tokens[i]);
    }
    if (tmp_pwd[0] != '/') {
        res = mx_strjoin("/", tmp_pwd);
        free(tmp_pwd);
    }
    else
        res = tmp_pwd;
    mx_del_strarr(&tokens);
    return res;
}

int mx_chdir_L(char *path, t_info *info) {
    char *new_pwd = get_new_pwd(path, info);
    if (chdir(new_pwd) == -1) {
        mx_printerr(strerror(errno));
        write(2, "\n", 1);
        free(new_pwd);
        return 1;
    }
    free(info->old_pwd);
    info->old_pwd = info->pwd;
    free(info->pwdL);
    info->pwdL = new_pwd;
    free(info->pwdP);
    info->pwdP = getcwd(NULL, 0);
    info->pwd = mx_strdup(info->pwdL);
    return 0;
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
    char *path = (flags & 4) ? info->old_pwd : (argv[i] ? argv[i] : info->home);
    t_variable *temp = 0;
    int status  = 0;

    if (is_link(path) && (flags & 1) && (flags & 2) == 0) {
        fprintf(stderr, "cd: not a directory: %s\n", argv[i]);
        return 1;
    }
    if (flags & 2) {
        status = mx_chdir_P(path, info);
    }
    else
        status = mx_chdir_L(path, info);
    if (status == 0) {
        export_pwd_oldpwd(info);
        // temp = mx_return_list(info->var_tree, "OLDPWD", if_argv);
        // if ()info->old_pwd;

    }
    return status;
}

