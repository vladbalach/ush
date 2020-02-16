#include "ush.h"

static char **creat_comands(t_list **list_comands) {
    int sum_comand = mx_list_size(*list_comands) + 2;
    char **comands = (char **) malloc(sum_comand * sizeof(char *));
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

static void del_input(t_info *info) {
    t_input *input = info->input;
    mx_del_strarr(&(input->comands));
    free(input);
    info->input = 0;
}

static unsigned int read_keyboard(t_info *info) {
    unsigned int ch = 0;

    if ((ch = mx_getchar()) == 0) {
        mx_printerr("u$h: some troubeles with input!\n");
        exit(2);
    }
    if (info->input->if_  == 9 && ch != 9)
        mx_del_strarr(&(info->input->comand_tab));
    return ch;
}

int mx_input(t_info *info) {
    unsigned int ch = 0;
    char *chars = (char*)(&ch);
    info->input = create_input(info);
    int if_next = 3;

    while (if_next == 3) {
        if (chars[2] != 10 || chars[0] == 9 || chars[0] == 18)
            mx_out_monitor_new(NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
        ch = read_keyboard(info);
        if (ch > 127)
            mx_ctrl_v_and_not_ascii(info, chars);
        else
            if_next = mx_ascii(info, chars, ch);
        info->input->if_ = chars[0];
    }
    del_input(info);
    return if_next;
}
