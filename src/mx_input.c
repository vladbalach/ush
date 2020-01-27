#include "ush.h"

int mx_handleEvents(char ch) {
    if (ch == KEY_ENTER) {
        return KEY_ENTER;
    }
    if (ch == CTRL_I)
        return 9;
    if (ch == CTRL_C || ch == CTRL_Z || ch == CTRL_D) {
        return -1; // exit
    }
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

static void mx_ctrl_R(char *name, char **comands, int *table){
    unsigned int ch = 100;
    char *temp = mx_strnew(0);
    int i = 1;
    bool check = 1;
    int index = table[1] + 1;

    while (check) {
    if (index != table[1] + 1)
        mx_out_monitor_new(name, mx_strlen(comands[index]) + 1, 0, comands[index]);
    else
        mx_out_monitor_new(name, 1, 0, "");
    mx_printstr("\n");
    mx_out_monitor_new("Search > ", i, 0, temp);
    ch = mx_getchar();
    mx_clean_monitor_new("Search > ", i, 0, temp);
    mx_print_esc("1A");
    if (ch > 31 && ch < 128)
        mx_one_symbol(&temp, ch, &i, 0);
    else
        check = 0;
    if (index != table[1] + 1) 
        mx_clean_monitor_new("u$h> ", mx_strlen(comands[index]) + 1, 0, comands[index]);
    else
        mx_clean_monitor_new("u$h> ", 1, 0, "");
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
}


static void special_symbols(char **comands, int *table, unsigned int ch, char ***comand_tab) {
    if (table[4] != 9 && ch == 9) {
        mx_clean_monitor_new("u$h> ", table[2], table[3], comands[table[0]]);
        *comand_tab = mx_key_tab(mx_strndup(comands[*table],table[2] - table[3] - 1), table, &comands[table[0]]);
        table[5] = 0;
    }
    if (table[4] == 9 && ch == 9) {
        mx_clean_monitor_new("u$h> ", table[2], table[3], comands[table[0]]);
        mx_key_duble_tab(&comands[table[0]], *comand_tab, table);
    }
        table[4] = mx_handleEvents(ch);
    if (table[4] == 18) {
        mx_clean_monitor_new("u$h> ", table[2], table[3], comands[table[0]]);
        mx_ctrl_R("u$h> ", comands, table);
    }
}



bool mx_input(t_list **list) {
    unsigned int ch = 0;
    char *chars = (char*)(&ch);
    int *table;
    char **comands = creat_comands(list, &mx_strdup, &table);
    char **comand_tab = NULL; 
    char *name = "u$h> ";

    while (1) {
        if (chars[2] != 10)
            mx_out_monitor_new(name, table[2], table[3], comands[table[0]]);
        if ((ch = mx_getchar()) == 0)
            mx_printerr("u$h: some troubeles with input!\n");
        if (table[4] == 9 && ch != 9)
            mx_del_strarr(&comand_tab);
        if (ch > 127) { // 2-4 symbols
            if (chars[0] == 27)
                mx_not_ascii(name, chars, table, comands);
            else {
                mx_clean_monitor_new(name, table[2], table[3], comands[table[0]]);
                for (int i = 0; i < 4 && chars[i] != 0; i++) {
                    mx_one_symbol(&comands[table[0]], chars[i], &table[2], table[3]);
                }
                mx_out_monitor_new(name, table[2], table[3], comands[table[0]]);
                chars[2] = 10;
            }
        }
        else { // 1 symbol
            if (ch < 32) {
                special_symbols(comands, table, ch, &comand_tab);
                if (table[4] == -1) { // CTRL_C | Z
                    free(table);
                    mx_del_strarr(&comands);
                    return 0;
                }
                if (table[4] == KEY_ENTER) {
                    mx_clean_monitor(comands[*table], table, comands[*table]);
                    if (mx_strlen(comands[table[0]]) != 0) {
                        if (*list == NULL || mx_strcmp(comands[*table], (*list)->data) != 0)
                            mx_push_front(list, mx_strdup(comands[*table]));
                        free(table);
                        mx_del_strarr(&comands);
                        return 1;
                    }
                }
            }
            else {
                mx_clean_monitor_new(name, table[2], table[3], comands[table[0]]);
                mx_one_symbol(&comands[table[0]], ch, &table[2], table[3]);
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
