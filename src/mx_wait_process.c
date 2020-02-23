#include "ush.h"
#include "macroses.h"

static void check_status(char **argv, int status, t_info *info, pid_t pr) {
    char **mas_name = 0;

    if (MX_WIFSIG(status)) {
        if (MX_WTERMSIG(status) == SIGSEGV)
            mx_segfault();
        if (MX_WTERMSIG(status) == SIGINT) {
            mx_del_pid_process(info, pr);
            info->lastStatus = 130;
        }
    }
    else {
        mx_add_process(&(info->processes), pr, argv);
        mas_name = mx_get_name(info, pr);
        mx_print_susp(mas_name);
    }
}

void mx_wait_process(t_info *info, char **argv) {
    int status = 0;
    pid_t pr = 0;

    pr = waitpid(-1, &status, WUNTRACED); 
    if (!MX_WIFEXIT(status)) {
        check_status(argv, status, info, pr);
    }
    else {
        mx_del_pid_process(info, pr);
        info->lastStatus = MX_EXSTATUS(status);
    }
}
