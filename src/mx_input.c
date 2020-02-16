#include "ush.h"

int mx_handleEvents(char ch) {
    if (ch == KEY_ENTER) {
        return KEY_ENTER;
    }
    if (ch == CTRL_I)
        return 9;
    if (ch == CTRL_D) {
        return -1; // exit
    }
    if (ch == CTRL_Z) {
        write(1, "\a", 1);
        // return 2;
    }
    if (ch == CTRL_C)
        return 2;
    if (ch == CTRL_R)
        return 18;
    return 0;
}

static void mx_ctrl_R(t_info *info){
    unsigned int ch = 100;
    char *temp = mx_strnew(0);
    int i = 1;
    bool check = 1;
    int index = MX_MAX_COMAND + 1;

    while (check) {
    if (index != MX_MAX_COMAND + 1)
        mx_out_monitor_new(NAME, mx_strlen(info->input->comands[index]) + 1, 0, info->input->comands[index]);
    else
        mx_out_monitor_new(NAME, 1, 0, "");
    mx_printstr("\n");
    mx_out_monitor_new(SEARCH, i, 0, temp);
    ch = mx_getchar();
    mx_clean_monitor_new(SEARCH, i, 0, temp);
    mx_print_esc("1A");
    if (ch > 31 && ch < 128)
        mx_one_symbol(&temp, ch, &i, 0);
    else
        check = 0;
    if (index != MX_MAX_COMAND + 1) 
        mx_clean_monitor_new(NAME, mx_strlen(info->input->comands[index]) + 1, 0, info->input->comands[index]);
    else
        mx_clean_monitor_new(NAME, 1, 0, "");
    index = MX_MAX_COMAND + 1;
    for (int y = 0; index == MX_MAX_COMAND + 1 && info->input->comands[y]; y++)
        if (mx_strstr(info->input->comands[y], temp) != 0)
            index = y;
    }
    if (index != MX_MAX_COMAND + 1) {
        MX_ID = index;
        MX_STR_POS = 0;
        MX_STR_LEN = mx_strlen(MX_STR) + 1;
    }
    mx_strdel(&temp);
}


static void special_symbols(unsigned int ch, t_info *info) {
    // char **s =0;
    if (MX_SYMBOL != 9 && ch == 9) {
        mx_clean_monitor_new(NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
        info->input->comand_tab = mx_key_tab(mx_strndup(MX_STR,MX_STR_LEN - MX_STR_POS - 1), &MX_STR, info);
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

static char **creat_comands(t_list **list_comands) {
    char **comands = (char **) malloc((mx_list_size(*list_comands) + 2) * sizeof(char *));
    t_list *list = *list_comands;

    comands[0] = mx_strnew(0);
    comands[1] = NULL;
    for (int i = 1; list != 0; i++) {
        comands[i] = mx_strdup(list->data);
        list = list->next;
        comands[i + 1] = NULL;
    }
    return comands;
}

static t_input *create_input(t_info *info) {
    t_input *input = (t_input *) malloc(sizeof(t_input));

    input->comands = creat_comands(&(info->history));
    input->id = 0;
    input->max_comand = mx_list_size(info->history);
    input->str_len = 1;
    input->pos_tab = 0;
    input->end_posit = 0;
    input->if_ = 0;
    return input;
}


void mx_out_monitor(char *name, int table2, int pos, char *str) {
    struct winsize w;
    int symbol = mx_bit_sumbol(&str[table2 - pos - 1]);
    int len = (int) name[0];// mx_strlen(name) - 1;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    mx_printstr(&name[1]);
    mx_printstr(str);
    mx_printstr(" ");
    for (int i = (mx_len_symbol(table2, str) + len) / w.ws_col; i > 0; i--)
        mx_print_esc("1F");
    write(1,"\r",1);
    mx_printstr(&name[1]);
    write(1, str, table2 - pos - 1);
    if ((mx_len_symbol(table2 - pos, str) + len) % w.ws_col == 0) {
        if (pos == 0)
            write(1, " ", 1);
        else
            write(1, &str[table2 - pos - 1], symbol);
    write(1, "\b", 1);
    }
    mx_printstr("\x1b[0m");
}

static void del_input(t_info *info) {
    t_input *input = info->input;
    mx_del_strarr(&(input->comands));
    free(input);
    info->input = 0;
}

int mx_input(t_info *info) {
    unsigned int ch = 0;
    char *chars = (char*)(&ch);
    info->input = create_input(info);

    while (1) {
        if (chars[2] != 10 || chars[0] == 9 || chars[0] == 18)
            mx_out_monitor_new(NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
        if ((ch = mx_getchar()) == 0) {
            mx_printerr("u$h: some troubeles with input!\n");
            exit(2);
        }
        if (info->input->if_  == 9 && ch != 9)
            mx_del_strarr(&(info->input->comand_tab));
        if (ch > 127) { // 2-4 symbols
            if (chars[0] == 27)
                mx_not_ascii(chars, info);
            else {
                mx_clean_monitor_new(NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
                for (int i = 0; i < 4 && chars[i] != 0; i++)
                    mx_one_symbol(&(MX_STR), chars[i], &(MX_STR_LEN), MX_STR_POS);
            }
        }
        else { // 1 symbol
            if (ch < 32) {
                special_symbols(ch, info);
                if (MX_SYMBOL == -1) { // CTRL_D | Z
                    mx_clean_monitor(MX_STR, info, "exit");
                    del_input(info);
                    return 0;
                }
                else if (MX_SYMBOL == 2) {
                    mx_clean_monitor_new(NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
                    del_input(info);
                    return 2;
                }
                else if (MX_SYMBOL == KEY_ENTER) {
                    mx_clean_monitor(MX_STR, info, MX_STR);
                    if (mx_strlen(MX_STR) != 0) {
                        if (info->history == NULL || mx_strcmp(MX_STR, info->history->data) != 0)
                            mx_push_front(&info->history, mx_strdup(MX_STR));
                        del_input(info);
                        return 1;
                    }
                }
                else
                    chars[2] = 10;
            }
            else {
                mx_clean_monitor_new(NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
                mx_one_symbol(&(MX_STR), ch, &(MX_STR_LEN), MX_STR_POS);
            }
        }
        info->input->if_ = chars[0];
    }
}
