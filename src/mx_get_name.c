#include "ush.h"

char **mx_get_name(t_info *info, int numb) {
    t_list *head_pr = info->processes;

    while (head_pr) {
        if (((t_process*)head_pr->data)->pid == numb) {
            return ((t_process*)head_pr->data)->name;
        }
        head_pr = head_pr->next;
    }
    return 0;
}
