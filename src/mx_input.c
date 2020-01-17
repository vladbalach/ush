#include "ush.h"

static void escape_sequences(char *chars, int *table,  char **comands) {
    if (chars[2] == 65) { // UP
        if (table[0] != table[1]) {
            clean_monitor(comands[table[0]], table, comands[++table[0]]);
            table[3] = 0;
            table[2] = mx_strlen(comands[table[0]]) + 1;
        }
        else 
            chars[2] = 10;
    }
    if (chars[2] == 66) { //DOWN
        if (table[0] != 0) {
            clean_monitor(comands[table[0]], table, comands[--table[0]]);
            table[3] = 0;
            table[2] = mx_strlen(comands[table[0]]) + 1;
        }
        else 
            chars[2] = 10;
    }
    if (chars[2] == 67) { // RIGHT
        if (0 < table[3]) {
            write(1, chars, 3);
            table[3]--; 
        }
        else 
            chars[2] = 10;
    }
    if (chars[2] == 68) { // LEFT
        if (table[2] - 1 > table[3]) {
            write(1, chars, 3);
            table[3]++;
        }
        else 
            chars[2] = 10;
    }
}



int mx_handleEvents(char ch) {
    if (ch == KEY_ENTER) {
        return KEY_ENTER;
    }
    if (ch == CTRL_C || ch == CTRL_Z || ch == CTRL_D) {
        return -1; // exit
    }
    return 0;
}

void one_symbol(char **str, char ch, int *count, int position) {
    int i = 0;

    if (ch == 127) {
        if ((*count) > 1 && (*count) > position + 1) {
            (*count)--;
            i = position;
            while (i > 0) {
                (*str)[(*count) - i - 1] = (*str)[(*count) - i];
                i--;
            }
            (*str)[(*count) - 1] = 0;
        }
    }
    else {
        // write(1, &ch, 1);
        (*count)++;
        *str = realloc(*str, *count);
        while (i <= position) {
            (*str)[(*count) - i - 1] = (*str)[(*count) - i - 2];
            i++;
        }
        (*str)[(*count) - position - 2] = ch;
    }
}

static char **creat_comands(t_list **list_comands, char *(*creat)(const char *s), int **table) {
    char **comands = (char **) malloc((mx_list_size(*list_comands) + 2) * sizeof(char *));
    t_list *list = *list_comands;
    int *table_int = (int *) malloc (5 * sizeof (int));

    table_int[0] = 0;
    table_int[1] = mx_list_size(*list_comands);
    table_int[2] = 1;
    table_int[3] = 0;
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
/*******************************************/

bool mx_input(t_list **list) {
    unsigned int ch = 0;
    char *chars = (char*)(&ch);
    int *table;
    char **comands = creat_comands(list, &mx_strdup, &table);

    mx_printstr("u$l = ");
    while (1) {
        if ((ch = mx_getchar()) == 0)
            mx_printerr("uSh: some troubeles with input!\n");
        if (ch > 127) { // 2-4 symbols
            if (chars[0] == 27 && chars[2] > 64 && chars[2] < 69)
                escape_sequences(chars, table, comands);
        }
        else { // 1 symbol
            if (ch < 32) {
                table[4] = mx_handleEvents(ch);
                if (table[4] == -1) { // CTRL_C | Z
                    free(table);
                    mx_del_strarr(&comands);
                    return 0;
                }
                if (table[4] == KEY_ENTER) {
                    clean_monitor(comands[*table], table, comands[*table]);
                    write(1, "\0\n", 2);
                    if (mx_strlen(comands[table[0]]) != 0) {
                        mx_push_front(list, mx_strdup(comands[*table]));
                        free(table);
                        mx_del_strarr(&comands);
                        return 1;
                    }
                }
            }
            else
                one_symbol(&comands[table[0]], ch, &table[2], table[3]);
        }
        if (chars[2] != 10)
            out_monitor(table[3], comands[table[0]], table[2], ch);
    }
}
