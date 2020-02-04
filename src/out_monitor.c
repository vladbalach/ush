#include "ush.h"

void mx_print_esc(char *s) {
    char temp = 27;

    write(1, &temp, 1);
    write(1, "[", 1);
    mx_printstr(s);
}

void mx_check_outprogram_new_line(void) {
    struct winsize w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    write(1,"%",1);
    for(int i = 0; i < w.ws_col - 1; i++)
        write(1," ",1);
    write(1,"\r",1);
    mx_print_esc("J");
}

void mx_out_monitor_new(char *name, int table2, int pos, char *str) {
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

void mx_clean_monitor_new(char *name, int table2, int pos, char *str) {
    struct winsize w;
    int temp;
    int len = (int) name[0];

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    temp = (mx_len_symbol(table2 - pos, str) + len) / w.ws_col;
    for (int i = temp; i > 0; i--) {
        mx_print_esc("1F");
    }
    write(1,"\r",1);
    mx_print_esc("J");
}

void mx_clean_monitor(char *str, int *table, char *new_str) {
    struct winsize w;
    int temp;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    temp = (mx_len_symbol(table[2] - table[3], str) + 4) / w.ws_col;
    for (int i = temp; i > 0; i--) {
        mx_print_esc("1F");
    }
    write(1,"\r",1);
    mx_print_esc("J");
    mx_printstr(HISTORY_STRING);
    mx_printstr(new_str);
    mx_printstr(" \x1b[0m\n");
}
