#include "ush.h"

/* 
*  1-st bit -> -a
*  2-nd bit -> -s
* -1 - error
*/
static int get_flags(int *i, char **argv) {
    int flags = 0;

    while(argv[++(*i)])
        if (argv[*i][0] == '-') {
            for(int j = 1; j < mx_strlen(argv[*i]); j++)
                if (argv[*i][j] == 'a')
                    flags |= 1;
                else if(argv[*i][j] == 's')
                    flags |= 2;
                else {
                    mx_printerr("which: bad option: -");
                    write(2, &argv[*i][j], 1);
                    write(2, "\n", 1);
                    return -1;
                }
        }
        else
            return flags;
    return flags;
}

/* 
1 - return true;
0 - dont return;
*/
static int mx_check_buildin(char *command, int flags, bool *finded) {
    if (mx_is_buildin(command)) {
        if ((flags & 2) == 2)
            return 1;
        printf("%s: shell built-in command\n", command);
        *finded = true;
        if ((flags & 1) == 0)
            return 1;
    }
    return 0;
}


static bool check_command(char *command, char **pathes, int flags) {
    char *fullname = 0;
    bool finded = false;

    if (mx_check_buildin(command, flags, &finded) == 1)
        return true;
    if (command[0] == '/') {
        if (mx_is_commad(mx_strdup(command), flags))
                return true;
    }
    else {
        for(int i = 0; pathes[i]; i++) {
            fullname = mx_strjoin2(mx_strjoin(pathes[i], "/"), command);
            if (mx_is_commad(fullname, flags)) {
                if ((flags & 1) == 0)
                    return true;
                finded = true;
            }
        }
    }
    return finded;
}

static bool check_commands(char **commands, char** pathes, int startIndex,
                           int flags) {
    int i = startIndex - 1;
    bool finded = false;

    while(commands[++i] != 0) {
        finded = check_command(commands[i], pathes, flags);
        if ((finded == false) && ((flags & 2) == 2))
            return false;
        if (finded == false) 
            printf("%s not found\n", commands[i]);
        if (finded && ((flags & 1) == 0 || (flags & 2) == 2))
            return true;
    }
    return finded;
}

void mx_which(char **argv, t_info *info) {
    char *path = mx_return_value2("PATH", &(info->var_tree));
    char **pathes = mx_strsplit(path, ':');
    int iArgs = 0;
    int flags = get_flags(&iArgs, argv);
    int finded = false;
    
    if (flags == -1) {
        mx_del_strarr(&pathes);
        info->lastStatus = 1;
        return;
    }
    finded = check_commands(argv, pathes, iArgs, flags);
    finded ? (info->lastStatus = 0) : (info->lastStatus = 1);
    mx_del_strarr(&pathes);
}
