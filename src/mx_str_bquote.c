#include "ush.h"

static char *read_to_delim(int des) {
    int count = 1;
    char buff[2];
    char *newStr = 0;

    buff[1] = 0;
    while((count = read(des, buff, 1))) {
        newStr = mx_strjoin2(newStr, buff);
    }
    // if (newStr && (mx_strlen(newStr) - 1) != -1)
    //     newStr[mx_strlen(newStr) - 1] = 0;
    close(des);
    // mx_printstr("1");
    // mx_printstr(newStr);
    // mx_printstr("2");
    // if (mx_strlen(newStr) == 0) {
    //     mx_strdel(&newStr);
    // }
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
