#include "ush.h"

static bool cmp_str_min_max(void *str1, void *str2) {
    if (mx_strcmp(str1, str2) > 0)
        return 1;
    else 
        return 0;
}

char **mx_key_tab(char *parsing, char **str, t_info *info) {
    char *path = mx_mini_parser_tab(parsing, info);
    t_list *list_comand = 0;
    char **creat_list_comands = NULL;
    int i = 0;
    int temp = 0;

    mx_read_comand(path, &list_comand);
    list_comand = mx_sort_list(list_comand, &cmp_str_min_max);
    if (!((temp = mx_list_size(list_comand)) == 0 || temp == 1)) {
        mx_clean_monitor("", info, *str);
        mx_print_Tab_comands(list_comand);
        creat_list_comands = (char **)malloc((mx_list_size(list_comand) + 1) * sizeof(char *));
        for (temp = mx_strlen(path) - 1; temp != 0 && path[temp] != '/'; temp--);
        temp = mx_strlen(&path[temp]);
        free(path);
        free(parsing);
        while (list_comand) {
            path = list_comand->data;
            creat_list_comands[i++] = mx_strdup(&path[temp - 1]);
            mx_pop_front(&list_comand);
        }
        creat_list_comands[i++] = NULL;
    }
    else if (temp == 1) {
        for (temp = mx_strlen(path) - 1; temp != 0 && path[temp] != '/'; temp--);
        temp = mx_strlen(&path[temp]);
        free(path);
        free(parsing);
        path = list_comand->data;
        for (i = temp - 1; path[i]; i++)
            mx_one_symbol(str, path[i], &(MX_STR_LEN), MX_STR_POS);
        if (MX_STR_POS != 0) {
            mx_one_symbol(str, ' ', &(MX_STR_LEN), MX_STR_POS);
            (MX_STR_POS)++;
        }
        mx_pop_front_free_data(&list_comand);
    }
    else {
        write(1,"\a", 1);
        free(path);
        free(parsing);
    }
    return creat_list_comands;
}
