#include "ush.h"

static int mx_handleEvents(char ch) {
    if (ch == KEY_ENTER) {
        return KEY_ENTER;
    }
    if (ch == CTRL_I)
        return 9;
    if (ch == CTRL_D) {
        return -1;
    }
    if (ch == CTRL_Z) {
        write(1, "\a", 1);
    }
    if (ch == CTRL_C)
        return 2;
    if (ch == CTRL_R)
        return 18;
    return 0;
}

static void special_symbols(char **comands, unsigned int ch, t_info *info) {
    int str_pos = MX_STR_LEN - MX_STR_POS - 1;
    char *str_tab = 0;

    if (MX_SYMBOL != 9 && ch == 9) {
        str_tab = mx_strndup(MX_STR,str_pos);
        mx_clean_monitor_new(NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
        info->input->comand_tab = mx_key_tab(str_tab, &MX_STR, info);
        info->input->pos_tab = 0;
    }
    if (MX_SYMBOL == 9 && ch == 9) {
        mx_clean_monitor_new(NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
        mx_key_duble_tab(&MX_STR, info->input->comand_tab, info);
    }
    MX_SYMBOL = mx_handleEvents(ch);
    if (MX_SYMBOL == 18) {
        mx_clean_monitor_new(NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
        mx_ctrl_R(info);
    }
}

static int ctrl_Enter_D_C(t_info *info) {
    if (MX_SYMBOL == -1) {
        mx_clean_monitor(MX_STR, info, "exit");
        return 0;
    }
    else if (MX_SYMBOL == 2) {
        mx_clean_monitor_new(NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
        return 2;
    }
    else if (MX_SYMBOL == KEY_ENTER) {
        mx_clean_monitor(MX_STR, info, MX_STR);
        if (mx_strlen(MX_STR) != 0) {
            if (info->history == NULL
                || mx_strcmp(MX_STR, info->history->data) != 0)
                mx_push_front(&info->history, mx_strdup(MX_STR));
            return 1;
        }
    }
    return 3;
}

int mx_ascii(t_info *info, char *chars, unsigned int ch) {
    int spec_symbol = 3;

    if (ch < 32) {
        special_symbols(info->input->comands, ch, info);
        if (MX_SYMBOL == -1 || MX_SYMBOL == 2 || MX_SYMBOL == 13)
            spec_symbol = ctrl_Enter_D_C(info);
        else
            chars[2] = 10;
    }
    else {
        mx_clean_monitor_new(NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
        mx_one_symbol(&(MX_STR), ch, &(MX_STR_LEN), MX_STR_POS);
    }
    return spec_symbol;
}
