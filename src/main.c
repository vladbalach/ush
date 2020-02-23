#include "ush.h"

int main(int argc, char *argv[], char **envp) {
    int str = 1;
    t_info *info = 0;

    mx_get_twidth();
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
    mx_ush_close(info);
    system("leaks ush");
    return info->exit_status;
}
