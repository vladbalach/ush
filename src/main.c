#include "ush.h"

int main(int argc, char *argv[], char **envp) {

    int str = 1;
    // t_list *history = NULL;
    // char **commands  = NULL;
    t_info *info = 0;
    argv++;
    argc++;

    mx_ush_init(&info, envp);
    while(str != 0) {
        if (info->history && str == 1) {
            mx_parsing(info->history->data, info);
            mx_check_outprogram_new_line();
        }
        if (info->isExit)
            break;
        str = mx_input(info);
    }
    // mx_ush_close(info);
    system("leaks ush");
    return info->exit_status;
}

// void add(t_info *info) {
//     t_process *pr = (t_process*) malloc (sizeof(t_process));
//     pr->index = 1;
//     pr->pid = 554;

//     mx_push_front(&(info->processes), pr);

    
// }


// int main(int argc, char *argv[], char **envp) {
//     t_info *info;
//     info = (t_info*) malloc (sizeof(t_info));
//     add(info);
//     printf("%d\n", ((t_process*)info->processes->data)->pid);
//     return 0;
// }

