#include "ush.h"

static unsigned int out_monitor_and_read_keyboard(t_info *info, int index,
                                                  int i, char *temp) {
    unsigned int ch = 0;

    if (index != MX_MAX_COMAND + 1)
        mx_out_monitor_new(NAME, mx_strlen(MX_COMMAND[index]) + 1, 0,
                           MX_COMMAND[index]);
    else
        mx_out_monitor_new(NAME, 1, 0, "");
    mx_printstr("\n");
    mx_out_monitor_new(SEARCH, i, 0, temp);
    ch = mx_getchar();
    mx_clean_monitor_new(SEARCH, i, 0, temp);
    mx_print_esc("1A");
    return ch;
}

static int search_comand(t_info *info, int index, char *temp) {
    int index_comand = MX_MAX_COMAND + 1;
    int str_len_comand = 0;

    if (index != MX_MAX_COMAND + 1) {
        str_len_comand = mx_strlen(MX_COMMAND[index]) + 1;
        mx_clean_monitor_new(NAME, str_len_comand, 0, MX_COMMAND[index]);
    }
    else
        mx_clean_monitor_new(NAME, 1, 0, "");
    for (int y = 0; index_comand == MX_MAX_COMAND + 1 && MX_COMMAND[y]; y++)
        if (mx_strstr(MX_COMMAND[y], temp) != 0)
            index_comand = y;
    return index_comand;
}

void mx_ctrl_R(t_info *info) {
    unsigned int ch = 0;
    char *temp = mx_strnew(0);
    int i = 1;
    bool check = 1;
    int index = MX_MAX_COMAND + 1;

    while (check) {
        ch = out_monitor_and_read_keyboard(info, index, i, temp);
        if (ch > 31 && ch < 128)
            mx_one_symbol(&temp, ch, &i, 0);
        else
            check = 0;
        index = search_comand(info, index, temp);
    }
    if (index != MX_MAX_COMAND + 1) {
        MX_ID = index;
        MX_STR_POS = 0;
        MX_STR_LEN = mx_strlen(MX_STR) + 1;
    }
    mx_strdel(&temp);
}
