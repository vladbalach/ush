#include "ush.h"

/* 
*  1-st bit -> -a
*  2-nd bit -> -s
* -1 - error
*/
// static void printErr(char c) {
//     mx_printerr("which: bad option: -");
//     write(2, &c, 1);
//     write(2, "\n", 1);
// }

static int get_flags(int *i, char **argv) {
    int flags = 0;
    int len = 0;

    while(argv[++(*i)])
        if (argv[*i][0] == '-') {
            len = mx_strlen(argv[*i]);
            for(int j = 1; j < len; j++)
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

static bool is_commad(char *fullname, int flags) {
    struct stat st;

    if (stat(fullname, &st) != -1) { // finded
        if ((st.st_mode & S_IXUSR) == S_IXUSR) {
            if ((flags & 2) == 2) {
                free(fullname);
                return true;
            }
            mx_printstr(fullname);
            mx_printchar('\n');
            free(fullname);
            return true;
        }
    }
    free(fullname);
    return false;
}

static bool check_command(char *command, char** pathes, int flags) {
    int i = -1;
    char *fullname = 0;
    bool finded = false;

    if (mx_is_buildin(command)) {
        if ((flags & 2) == 2)
            return true;
        printf("%s: shell built-in command\n", command);
        finded = true;
        if ((flags & 1) == 0)
            return true;
    }
    while (pathes[++i]) {
        fullname = mx_strjoin(pathes[i], "/");
        fullname = mx_strjoin2(fullname, command);
        if (is_commad(fullname, flags)) {
            if ((flags & 1) == 0)
                return true;
            finded = true;
        }
    }
    return finded;
}


// static bool check_command(char *command, char** pathes, int flags) {
//     int i = -1;
//     char *fullname = 0;
//     struct stat st;
//     bool finded = false;

//     if (mx_is_buildin(command)) {
//         if ((flags & 2) == 2)
//             return true;
//         printf("%s: shell built-in command\n", command);
//         finded = true;
//     }
//     if (((flags & 1) == 0) && (finded))
//             return true;
//     while (pathes[++i]) {
//         fullname = mx_strjoin(pathes[i], "/");
//         fullname = mx_strjoin2(fullname, command);
//         if (stat(fullname, &st) != -1) { // finded
//             if ((st.st_mode & S_IXUSR) == S_IXUSR) {
//                 finded = true;
//                 if ((flags & 2) == 2) {
//                     free(fullname);
//                     return 1;
//                 }
//                 mx_printstr(fullname);
//                 mx_printchar('\n');
//                 if ((flags & 1) == 0) {
//                     free(fullname);
//                     return finded;
//                 }
//             }
//         }
//         free(fullname);
//     }
//     return finded;
// }

static bool check_commands(char **commands, char** pathes, 
int startIndex, int flags) 
{
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
    char **pathes = mx_strsplit(info->path, ':');
    int iArgs = 0;
    int flags = get_flags(&iArgs, argv);
    int finded = false;

    if (flags == -1){
        mx_del_strarr(&pathes);
        return;
    }
    finded = check_commands(argv, pathes, iArgs, flags);

    mx_del_strarr(&pathes);
}
