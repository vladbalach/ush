#include "ush.h"

int mx_get_pr_index(pid_t *processes, pid_t pid) {
    int i = 0;

    for (i = 0; i < MAX_PROC_COUNT; i++)
        if (processes[i] == 0)
            return i;
    for (i = 0; i < MAX_PROC_COUNT; i++)
        if (getpgid(processes[i]) == -1) {
            return i;
        }
    return -1;
}
