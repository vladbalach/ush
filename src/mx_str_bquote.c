#include "ush.h"

static char *read_to_delim(int des) {
    int count = 1;
    char buff[2];
    char *newStr = 0;

    buff[1] = 0;
    while((count = read(des, buff, 1))) {
        if (buff[0] == '\n')
            buff[0] = '\x0d';
        newStr = mx_strjoin2(newStr, buff);
    }
    if (newStr) {
        count = strlen(newStr);
        if (newStr[count] == '\x0d')
            newStr[count] = 0;
        if (count > 1 && newStr[count - 1] == '\x0d')
            newStr[count - 1] = 0;
    }
    close(des);
    return newStr;
}

char *mx_str_bquote(char **str, t_info *processes) {
    int des[2] = {0, 0};
    pid_t pid;

    pipe(des);
    pid = fork();
    if (pid == 0) {
        dup2(des[1], 1);
        mx_parsing(*str, processes);
        exit(1);
    }
    else if (pid == -1) {
        mx_printerr(strerror(errno));
        exit(1);
    }
    else
        wait(0);
    close(des[1]);
    mx_strdel(str);
    return read_to_delim(des[0]);
}
