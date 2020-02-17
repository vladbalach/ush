#include "ush.h"

void mx_jobs(t_info *info) {
    t_list *tmp = info->processes;
    char *name = 0;
    char c = 0;
    int i = -1;

    if (tmp == 0)
        printf("fg: no current job\n");
    while (tmp) {
        name = mx_arrstr_to_str(((t_process*)tmp->data)->name);
        if (++i == 0)
            c = '+';
        else if (i == 1)
            c = '-';
        else 
            c = ' ';
        printf("[%d] %c suspended %s\n", ((t_process*)tmp->data)->index, c,
        name);
        free(name);
        tmp = tmp->next;
    }
}
