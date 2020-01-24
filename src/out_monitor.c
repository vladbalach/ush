#include "ush.h"

static void print_esc(char *s) {
    char temp = 27;

    write(1, &temp, 1);
    write(1, "[", 1);
    mx_printstr(s);
}

void out_monitor(int position, char *str, int count, int ch) {
    struct winsize w;
    int temp;
    char *chars = (char*)(&ch);

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    temp = (count + 5 - position) / w.ws_col;
    if ((count + 5 - position) % w.ws_col == 0 && !(chars[2] == 65 || chars[2] == 66))
        temp--;
    if  (((count + 5 - position) % w.ws_col == 0 && chars[2] == 68) || ((count + 5 - position) % w.ws_col  - w.ws_col == -1 && ch == 127))
        temp++;
    if ((count + 5 - position) % w.ws_col == 0 && ch == 127)
        temp++;
    for (int i = temp; i > 0; i--)
        print_esc("1A");
    write(1,"\r",1);
    print_esc("J");
    mx_printstr("u$h = ");
    mx_printstr(str);
    mx_printstr(" ");
    for (int i = (count + 5) / w.ws_col; i > 0; i--)
        print_esc("1A");
    write(1,"\r",1);
    mx_printstr("u$h = ");
    write(1, str, count - position - 1);
    if ((count + 5 - position) % w.ws_col == 0) {
        if (position == 0)
            write(1, " ", 1);
        else
            write(1, &str[count - position - 1], 1);
    write(1, "\b", 1);
    }
}

void clean_monitor(char *str, int *table, char *new_str) {
    struct winsize w;
    int temp;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    temp = (table[2] + 5 - table[3]) / w.ws_col;
    for (int i = temp; i > 0; i--) {
        print_esc("1A");
    }
    write(1,"\r",1);
    print_esc("J");
    mx_printstr("u$h = ");
    mx_printstr(new_str);
    mx_printstr(" ");
}

static void print_comands(int max, char *comands) {
    int char_comands = mx_strlen(comands) / 8;

    mx_printstr(comands);
    for (int i = char_comands; i != max; i++)
        mx_printchar('\t');
}

static int max_comands(t_list *list_comand) {
    t_list *comands = list_comand;
    int temp = 0;
    int max = 0;

    while (comands != 0) {
        temp = mx_strlen(comands->data);
        if (temp > max)
            max = temp;
        comands = comands->next;
    }
    return max;
}

void mx_print_Tab_comands(t_list *list_comand) {
    struct winsize w;
    int max;
    int coloms;
    int row;
    t_list *list = NULL;
    int temp;
    t_list *list2 = list_comand;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    max = max_comands(list_comand);
    if (max % 8 == 0)
        max += 8;
    else
        max = max + 8 - max % 8;
    coloms = w.ws_col / max;
    max /= 8;
    row = mx_list_size(list_comand) / coloms;
    if (mx_list_size(list_comand) % coloms != 0)
        row++;
    for (int i = w.ws_col; i != 0; i--)
        mx_printchar('=');
    if (list_comand == NULL)
        mx_printstr("\nNOT COMAND\n");
    for (int i = row; i != 0; i--) {
        list = list2;
        while (list != 0) {
            print_comands(max, list->data);
            for (temp = row; list != 0 && temp != 0; temp --)
                list = list->next;
        }
        mx_printchar('\n');
        list2 = list2->next;
    }
    for (int i = w.ws_col; i != 0; i--)
        mx_printchar('=');
}

