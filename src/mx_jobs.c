#include "ush.h"

static void print_row(t_list *tmp, char c) {
    char *name = mx_arrstr_to_str(((t_process*)tmp->data)->name);

    printf("[%d] %c suspended %s\n", ((t_process*)tmp->data)->index, c,
        name);
    free(name);
}

void mx_jobs(t_info *info) {
    t_list *tmp = info->processes;
    char c = 0;
    int i = -1;

    if (tmp == 0)
        printf("fg: no current job\n");
    while (tmp) {
        
        if (++i == 0)
            c = '+';
        else if (i == 1)
            c = '-';
        else 
            c = ' ';
        print_row(tmp, c);
        tmp = tmp->next;
    }
    info->last_status = 0;
}
