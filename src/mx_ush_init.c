#include "ush.h"

void mx_segfault() {
    mx_printerr("OOPS! SEGMENT FAULT\n");
    exit(1);
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
    newInfo->processes = 0;
    (*info) = newInfo;
    signal(SIGSEGV, mx_segfault);
}
