#include "ush.h"
#include "macroses.h"

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

static void child(t_info *processes, int des[2], char **str) {
    close(des[0]);
    dup2(des[1], 1);
    mx_parsing(*str, processes);
    if (processes->lastStatus == 130 || !(processes->if_ctrl_c))
        exit(130);
    exit(0);
}

static void parent(t_info *processes, int *status, char **str, int des[2]) {
    *status = MX_EXSTATUS(*status);
    mx_strdel(str);
    close(des[1]);
    if (*status == 130) {
        processes->if_ctrl_c = 0;
        close(des[0]);
    }
}

char *mx_str_bquote(char **str, t_info *processes) {
    int des[2] = {0, 0};
    pid_t pid;
    int status;

    pipe(des);
    pid = fork();
    if (pid == 0)
        child(processes, des, str);
    else if (pid == -1) {
        mx_printerr(strerror(errno));
        exit(1);
    }
    else
        wait(&status);
    parent(processes, &status, str, des);
    if (!processes->if_ctrl_c)
        return 0;
    return read_to_delim(des[0]);
}
