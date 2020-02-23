#include "ush.h"

static char **creat_comands(t_list **list_comands) {
    int sum_comand = mx_list_size(*list_comands) + 2;
    char **comands = (char **)malloc(sum_comand * sizeof(char *));
    t_list *list = *list_comands;

    comands[0] = NULL;
    for (int i = 0; list != 0; i++) {
        comands[i] = mx_strdup(list->data);
        list = list->next;
        comands[i + 1] = NULL;
    }
    return comands;
}

int mx_history(t_list **list_comands) {
    char **comands = creat_comands(list_comands);

    for (int i = 1; comands && comands[i - 1]; i++) {
        if (i < 10)
            mx_printstr("  ");
        else if (i < 100)
            mx_printstr(" ");
        mx_printint(i);
        mx_printstr("  ");
        mx_printstr(comands[i - 1]);
        mx_printstr("\n");
    }
    mx_del_strarr(&comands);
    return 0;
}
