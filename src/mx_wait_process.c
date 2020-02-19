#include "ush.h"
#include "macroses.h"

void mx_wait_process(t_info *info, char **argv) {
    int status = 0;
    pid_t pr = 0;
    char **mas_name = 0;

    pr = waitpid(-1, &status, WUNTRACED); 
    if (!MX_WIFEXIT(status)) {
        if (MX_WIFSIG(status)) {
            if(MX_WTERMSIG(status) == SIGSEGV)
                mx_segfault();
        }
        else {
            mx_add_process(&(info->processes), pr, argv);
            mas_name = mx_get_name(info, pr);
            mx_print_susp(mas_name);
        }
    }
    else {
        mx_del_top_process(info);
        info->lastStatus = MX_EXSTATUS(status);
    }
}
