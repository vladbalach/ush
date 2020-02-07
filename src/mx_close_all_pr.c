#include "ush.h"

void mx_close_all_pr(t_info *info) {
    for(int i = 0; i < MAX_PROC_COUNT; i++)
        if (info->processes[i] != 0)
            if (kill(info->processes[i], SIGKILL) == 0)
                printf("Process [%d] killed\n", info->processes[i]);
}