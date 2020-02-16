#include "ush.h"

void mx_close_all_pr(t_info *info) {
    t_list *tmp = info->processes;
    while(tmp) {
        if (kill(((t_process*)tmp->data)->pid, SIGKILL) == 0)
            printf("Process [%d] killed\n", ((t_process*)tmp->data)->pid);
        tmp = tmp->next;
    }
}
