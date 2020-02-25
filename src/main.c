#include "ush.h"

void mx_not_stdin_terminal(t_info *info, int argc, char *argv[]) {
    char *str = 0;
    char *temp = mx_strnew(1024);
    char *temp2 = 0;
    ssize_t check;
    int i = -1;

    info->if_ctrl_c = 1;
    while ((check = read(0, temp, 1024)) != 0) {
        temp2 = mx_strndup(temp, check);
        str = mx_strjoin3(str, temp2);
    }
    free(temp);
    while (str && str[++i] != 0)
        if (str[i] == '\n')
            str[i] = ';';
    mx_parsing(str, info);
    free(str);
    argv++;
    argc++;
}

int main(int argc, char *argv[], char **envp) {
    int str = 1;
    t_info *info = 0;

    mx_get_twidth();
    mx_ush_init(&info, envp);
    if (isatty(0) == 0) 
        mx_not_stdin_terminal(info, argc, argv);
    else
        while (str != 0) {
            if (info->history && str == 1) {
                mx_parsing(info->history->data, info);
                mx_check_outprogram_new_line();
            }
            if (info->is_exit)
                break;
            str = mx_input(info);
            info->if_ctrl_c = 1;
        }
    mx_ush_close(info);
    // system("leaks ush");
    return info->exit_status;
}
