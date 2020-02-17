#include "ush.h"

void mx_check_outprogram_new_line(void) {
    struct winsize w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    write(1,"%",1);
    for(int i = 0; i < w.ws_col - 1; i++)
        write(1," ",1);
    write(1,"\r",1);
    mx_print_esc("J");
}

static void print_two_str(char *str1, char *str2) {
    mx_printstr(str1);
    mx_printstr(str2);
}

void mx_out_monitor_new(char *name, int table2, int pos, char *str) {
    struct winsize w;
    int symbol = mx_bit_sumbol(&str[table2 - pos - 1]);
    int len = (int) name[0];

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    print_two_str(&name[1], str);
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

void mx_clean_monitor(char *str, t_info *info, char *new_str) {
    struct winsize w;
    int temp;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    temp = (mx_len_symbol(MX_STR_LEN - MX_STR_POS, str) + 4) / w.ws_col;
    for (int i = temp; i > 0; i--) {
        mx_print_esc("1F");
    }
    write(1,"\r",1);
    mx_print_esc("J");
    mx_printstr(MX_HISTORY_STRING);
    mx_printstr(new_str);
    mx_printstr(" \x1b[0m\n");
}
