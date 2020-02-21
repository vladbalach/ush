#include "ush.h"

void mx_del_pid_process(t_info *info, int pid) {
    t_list *tmp = info->processes;

    if (info->processes) {
        if (((t_process*)tmp->data)->pid == pid) {
            mx_del_top_process(info);
            return;
        }
        while (tmp->next) {
            if (((t_process*)tmp->next->data)->pid == pid) {
                tmp->next = tmp->next->next;
                return;
            }
            tmp = tmp->next;
        }
    }
}
