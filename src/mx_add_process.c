#include "ush.h"

int mx_add_process(pid_t *processes, pid_t pid) {
    int index = mx_get_pr_index(processes, pid);

    if (index == -1)
        return -1;
    processes[index] = pid;
    return index;
}
