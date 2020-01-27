#include "ush.h"

static void print_comands(int max, char *comands) {
    int char_comands = mx_strlen(comands);

    mx_printstr(comands);
    for (int i = char_comands; i != max; i++)
        mx_printchar(' ');
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

static int row_tab(int *max, t_list *list_comand) {
    struct winsize w;
    int coloms = 0;
    int row = 0;
    int sum = mx_list_size(list_comand);

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    coloms = w.ws_col / *max;
    if (coloms == 0)
        coloms = 1;
    row = sum / coloms;
    if (sum % coloms != 0)
        row++;
    return row;
}

void mx_print_Tab_comands(t_list *list_comand) {
    int max = max_comands(list_comand) + 2;
    int row = row_tab(&max, list_comand);
    t_list *list = NULL;
    t_list *list2 = list_comand;
    int i[] = {0,0};

    for (i[0] = row; i[0] != 0; i[0]--) {
        list = list2;
        while (list != 0) {
            print_comands(max, list->data);
            for (i[1] = row; list != 0 && i[1] != 0; i[1] --)
                list = list->next;
        }
        mx_printchar('\n');
        list2 = list2->next;
    }
}
