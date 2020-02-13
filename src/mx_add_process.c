#include "ush.h"

static int get_max_index(t_list* process) {
    t_list *tmp = process;
    int max = 0;

    if (tmp == 0)
        return -1;
    max = ((t_process*)tmp->data)->index;
    while (tmp) {
        if (max < ((t_process*)tmp->data)->index) {
            max = ((t_process*)tmp->data)->index;
        }
        tmp = tmp->next;
    }
    return max;
}

int mx_add_process(t_list **processes, pid_t pid) {
    int max_index = get_max_index(*processes);
    t_process *pr = (t_process*) malloc (sizeof(t_process));
    pr->index = max_index + 1;
    pr->pid = pid;
    mx_push_front(processes, pr);
    return max_index + 1;
}
