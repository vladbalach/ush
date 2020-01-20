#include "ush.h"

void mx_parsing(char *str, char **envp) {
    // char **comands = mx_strsplit(str, ' ');
    // if (mx_strcmp(comands[0], "cd") == 0) {
    //     mx_cd(comands[1]);
    //     mx_del_strarr(&comands);
    // }
    // else if (mx_strcmp(comands[0], "env") == 0) {
    //     mx_env(envp);
    //     mx_del_strarr(&comands);
    // }
    // else if (mx_strcmp(comands[0], "exit") == 0) {
    //     exit(0);
    // }
    // else {
    //     pid_t work = fork();
    //     int waiting;
    //     if (work == -1)
    //         mx_printerr("fork system call failed");
    //     else if (work == 0) {
    //         execvp(comands[0], comands);
    //         mx_printerr("u$h: command not found: ");
    //         mx_printerr(comands[0]);
    //         mx_printerr("\n");
    //         exit(0);
    //     }
    //     else {
    //         mx_del_strarr(&comands);
    //     // mx_printchar('1');
    //         while (wait(&waiting) != work);
    //     }
    // }
}
