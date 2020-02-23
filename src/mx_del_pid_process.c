#include "ush.h"


static void free_proc() {

}

void mx_del_pid_process(t_info *info, int pid) {
    t_list *tmp = info->processes;
    t_list *tmp2 = info->processes;

    if (info->processes) {
        if (((t_process*)tmp->data)->pid == pid) {
            mx_del_top_process(info);
            return;
        }
        while (tmp->next) {
            if (((t_process*)tmp->next->data)->pid == pid) {
                tmp2 = tmp->next;
                tmp->next = tmp->next->next;
                mx_del_strarr(&((t_process*)tmp2->next->data)->name);
                free(tmp2);
                return;
            }
            tmp = tmp->next;
        }
    }
}
