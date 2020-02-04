#include "ush.h"

void mx_segfault() {
    mx_printerr("OOPS! SEGMENT FAULT\n");
    exit(1);
}
void mx_ctrl_c() {
    // t_info *info = mx_get_info(0);
    // t_list *tmp = info->processes;
    // pid_t pid = 0;
    // while(tmp) {
    //     pid = (pid_t) tmp->data;
    //     kill(pid, SIGKILL);
    // }
    printf("CTRL+C\n");
}


static void init_var() {
    // int initCount = 10;
    
    // *variables = (t_variable **) malloc (sizeof(t_variable*) * initCount); // initCount -vars
    // for (int i = 0; i < initCount; i++) {
    //     (*variables)[i] = (t_variable *) malloc (sizeof(t_variable));
    // }
}

void mx_ush_init(t_info **info, char **env) {
    t_info *newInfo = (t_info*) malloc(sizeof(t_info));
    newInfo->env = env;
    newInfo->processes = (pid_t*) malloc(sizeof(pid_t) * 11);
    int i = 0;
    for(i = 0; i < 10; i++)
        newInfo->processes[i] = 0; // empty
    (*info) = newInfo;

    // mx_get_info(*info);
    signal(SIGSEGV, mx_segfault);
    signal(SIGINT, mx_ctrl_c);
}
