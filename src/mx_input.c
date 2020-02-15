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

static char **creat_comands(t_list **list_comands, char *(*creat)(const char *s), int **table) {
    char **comands = (char **) malloc((mx_list_size(*list_comands) + 2) * sizeof(char *));
    t_list *list = *list_comands;
    int *table_int = (int *) malloc (5 * sizeof (int));

    table_int[0] = 0;
    table_int[1] = mx_list_size(*list_comands);
    table_int[2] = 1;
    table_int[3] = 0;
    table_int[5] = 0;
    *table = table_int;
    comands[0] = mx_strnew(0);
    comands[1] = NULL;
    for (int i = 1; list != 0; i++) {
        comands[i] = creat(list->data);
        list = list->next;
        comands[i + 1] = NULL;
    }
    return comands;
}

/*******************************************/
/*         table[i]                        */
/*         0 = id current command          */
/*         1 = id max command              */
/*         2 = sum letter in command       */ //count
/*         3 = printing position           */ //position
/*         4 = if ch < 32                  */
/*         5 = position TAB                */
/*******************************************/

static void mx_ctrl_R(char **comands, int *table){
    unsigned int ch = 100;
    char *temp = mx_strnew(0);
    int i = 1;
    bool check = 1;
    int index = table[1] + 1;

    while (check) {
    if (index != table[1] + 1)
        mx_out_monitor_new(NAME, mx_strlen(comands[index]) + 1, 0, comands[index]);
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
    if (index != table[1] + 1) 
        mx_clean_monitor_new(NAME, mx_strlen(comands[index]) + 1, 0, comands[index]);
    else
        mx_clean_monitor_new(NAME, 1, 0, "");
    index = table[1] + 1;
    for (int y = 0; index == table[1] + 1 && comands[y]; y++)
        if (mx_strstr(comands[y], temp) != 0)
            index = y;
    }
    if (index != table[1] + 1) {
        table[0] = index;
        table[3] = 0;
        table[2] = mx_strlen(comands[table[0]]) + 1;
    }
    mx_strdel(&temp);
}


static void special_symbols(char **comands, int *table, unsigned int ch, t_info *info) {
    // char **s =0;
    if (table[4] != 9 && ch == 9) {
        mx_clean_monitor_new(NAME, table[2], table[3], comands[table[0]]);
        info->input->comand_tab = mx_key_tab(mx_strndup(comands[*table],table[2] - table[3] - 1), table, &comands[table[0]], info);
        table[5] = 0;
    }
    if (table[4] == 9 && ch == 9) {
        mx_clean_monitor_new(NAME, table[2], table[3], comands[table[0]]);
        mx_key_duble_tab(&comands[table[0]], info->input->comand_tab, table);
    }
    table[4] = mx_handleEvents(ch);
    if (table[4] == 18) {
        mx_clean_monitor_new(NAME, table[2], table[3], comands[table[0]]);
        mx_ctrl_R(comands, table);
    }
}

static t_input *create_input(t_info *info, int **table) {
    t_input *input = (t_input *) malloc(sizeof(t_input *));

    input->comands = creat_comands(&info->history, &mx_strdup, table);
    // input->id = 0;
    // input->max_comand = mx_list_size(info->history);
    // input->str_len = 1;
    // input->pos_tab = 0;
    // input->end_posit = 0;
    // input->if_ = 0;
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


int mx_input(t_info *info) {
    unsigned int ch = 0;
    char *chars = (char*)(&ch);
    int *table;
    info->input = create_input(info, &table);
    // info->input = (t_input *) malloc(sizeof(t_input *));
    // info->input->comands = creat_comands(&info->history, &mx_strdup, &table);

    while (1) {
        if (chars[2] != 10)
            mx_out_monitor_new(NAME, table[2], table[3], info->input->comands[table[0]]);
        if ((ch = mx_getchar()) == 0) {
            mx_printerr("u$h: some troubeles with input!\n");
            exit(2);
        }
        if (table[4] == 9 && ch != 9)
            mx_del_strarr(&info->input->comand_tab);
        if (ch > 127) { // 2-4 symbols
            if (chars[0] == 27)
                mx_not_ascii(chars, table, info->input->comands);
            else {
                mx_clean_monitor_new(NAME, table[2], table[3], info->input->comands[table[0]]);
                for (int i = 0; i < 4 && chars[i] != 0; i++)
                    mx_one_symbol(&info->input->comands[table[0]], chars[i], &table[2], table[3]);
            }
        }
        else { // 1 symbol
            if (ch < 32) {
                special_symbols(info->input->comands, table, ch, info);
                if (table[4] == -1) { // CTRL_D | Z
                    mx_clean_monitor(info->input->comands[*table], table, "exit");
                    free(table);
                    mx_del_strarr(&info->input->comands);
                    return 0;
                }
                if (table[4] == 2) {
                    mx_clean_monitor_new(NAME, table[2], table[3], info->input->comands[table[0]]);
                    free(table);
                    mx_del_strarr(&info->input->comands);
                    return 2;
                }
                if (table[4] == KEY_ENTER) {
                    mx_clean_monitor(info->input->comands[*table], table, info->input->comands[*table]);
                    if (mx_strlen(info->input->comands[table[0]]) != 0) {
                        if (info->history == NULL || mx_strcmp(info->input->comands[*table], info->history->data) != 0)
                            mx_push_front(&info->history, mx_strdup(info->input->comands[*table]));
                        free(table);
                        mx_del_strarr(&info->input->comands);
                        return 1;
                    }
                }
            }
            else {
                mx_clean_monitor_new(NAME, table[2], table[3], info->input->comands[table[0]]);
                mx_one_symbol(&info->input->comands[table[0]], ch, &table[2], table[3]);
            }
        }
        table[4] = chars[0];
        // mx_printint((unsigned char)chars[0]);
        // mx_printstr(" ");
        // mx_printint((unsigned char)chars[1]);
        // mx_printstr(" ");
        // mx_printint((unsigned char)chars[2]);
        // mx_printstr(" ");
        // mx_printint((unsigned char)chars[3]);
        // mx_printstr("\n");
        // if (chars[2] != 10) 
        //     out_monitor(table[3], comands[table[0]], table[2], ch);
        // mx_printstr("1");
    }
    
}
