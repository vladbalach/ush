#include "ush.h"

void mx_fg(t_info *info) {
    pid_t ch_pr = 0;
    int status = 0;

    if (info->processes) {
        ch_pr = ((t_process*)info->processes->data)->pid;
        mx_print_cont(mx_get_name(info, ch_pr),
            ((t_process*)info->processes->data)->index);
        kill(ch_pr, SIGCONT);
        ch_pr = waitpid(-1, &status, WUNTRACED);
        if (!WIFEXITED(status))
            mx_print_susp(mx_get_name(info, ch_pr));
        else {
            mx_del_top_process(info);
            info->lastStatus = WEXITSTATUS(status);
        }
    }
    else
        mx_printerr("fg: no current job\n");
}
