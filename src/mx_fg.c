#include "ush.h"

void mx_fg(t_info *info) {
    pid_t ch_pr = 0;
    int status = 0;
    char **mas_name = 0;
       
    if (info->processes) {
        ch_pr = ((t_process*)info->processes->data)->pid;
        mas_name = mx_get_name(info, ch_pr);
        mx_print_cont(mas_name, ((t_process*)info->processes->data)->index);
        
        kill(ch_pr, SIGCONT);
        ch_pr = waitpid(-1, &status, WUNTRACED);
        if (!WIFEXITED(status)) {
            mas_name = mx_get_name(info, ch_pr);
            mx_print_susp(mas_name);
        }
        else {
            mx_del_top_process(info);
            info->lastStatus = WEXITSTATUS(status);
        }
    }
    else
        mx_printerr("fg: no current job\n");
}
