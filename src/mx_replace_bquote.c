#include "ush.h"

/* 0 - no backquotes
* 1 - closed
* 2 - opened
*/
static char check_closed_bq(char *str) {
    char is_opened = 0;
    int i = 0;
    while (str[i]) {
        if ((str[i] == '`') && ((is_opened == 0) || (is_opened == 1)))
            is_opened = 2;
        else if(str[i] == '`' && is_opened == 2)
            is_opened = 1;
        i++;
    }
    return is_opened;
}
static char *read_to_delim(char *name, char delim) {
    int fd = open("tmp.ush", O_RDWR | O_CREAT);
    int count = 1;
    char buff[2];
    char *newStr = 0;

    buff[1] = 0;
    while((count = read(fd, buff, 1))) {
        if (buff[0] == delim)
            return newStr;
        newStr = mx_strjoin2(newStr, buff);
    }
    close(fd);
    return newStr;
}

static char* mx_do_bq(char **str, size_t start, size_t end, t_info *processes) {
    char *str_exec = mx_strndup(&(*str)[start], (end - start));
    char **commands = 0;
    char *str_out = 0;
    int fd = 0;
    pid_t pid = fork();

    if (pid == 0) {
        fd = open("tmp.ush", O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
        dup2(fd, 1);
        commands = mx_strsplit(str_exec, ';');
        mx_execute(commands, processes);
        mx_del_strarr(&commands);
        exit(1);
    }
    else if(pid == -1) {
        mx_printerr(strerror(errno));
        exit(1);
    }
    else
        wait(0);
        free(str_exec);
    return read_to_delim("temp.ush", '\n');
}

static void do_replace(char **str, size_t start, size_t end, char *str_new) {
    char *newStr = (char*) malloc (mx_strlen(*str) - (end - start) + mx_strlen(str_new));
    int i = 0;
    int j = 0;

    for(i = 0; i < start; i++) {
        newStr[i] = (*str)[i];
    }
    while(str_new[i - start]) {
        newStr[i] = str_new[i - start];
        i++;
    }
    j = end;
    while((*str)[j])
        newStr[i++] = (*str)[j++];
    newStr[i] = 0;
    free(*str);
    *str = newStr;
}

static void mx_find_replace(char **str, t_info *processes) {
    int i = 0;
    size_t startI = -1;
    size_t endI = -1;
    char *str_out = 0;

    while ((*str)[i]) {
        if ((*str)[i] == '`' && startI == -1)
            startI = i + 1;
        else if ((*str)[i] == '`' && startI != -1)
            endI = i;
        if(startI >= endI) {
            startI = -1;
            endI = -1; 
        }
        if (startI != -1 && endI != -1) {
            if ((str_out = mx_do_bq(str, startI, endI, processes)))
                do_replace(str, startI - 1, endI + 1, str_out);
            free(str_out);
            startI = -1;
            endI = -1;
        }
        i++;
    }
}

int mx_replace_bquote(char **str, t_info *processes) {
    char is_opened = check_closed_bq(*str);

    if (is_opened == 2){
        mx_printerr("Not closed bquote!\n");
        return 0;
    }
    if (is_opened == 1) {
        mx_find_replace(str, processes);
    }
    return 1;
}