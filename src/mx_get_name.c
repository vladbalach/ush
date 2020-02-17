#include "ush.h"

char **mx_get_name(t_info *info, int numb) {
    t_list *head_pr = info->processes;

    while (head_pr) {
        if (((t_process*)info->processes->data)->pid == numb) {
            return ((t_process*)info->processes->data)->name;
        }
        head_pr = head_pr->next;
    }
    return 0;
}
