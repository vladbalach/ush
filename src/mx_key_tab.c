#include "ush.h"

static bool cmp_str_min_max(void *str1, void *str2) {
    if (mx_strcmp(str1, str2) > 0)
        return 1;
    else 
        return 0;
}

static char **many_comand_in_list(char *path, char **str, t_info *info,
                                  t_list *list_comand) {
    int number_comand = 0;
    char **creat_list_comands = NULL;
    int temp = 0;
    int size_list = mx_list_size(list_comand) + 1;
    char *name_comand = 0;

    mx_clean_monitor("", info, *str);
    mx_print_Tab_comands(list_comand);
    creat_list_comands = (char **)malloc(size_list * sizeof(char *));
    for (temp = mx_strlen(path) - 1; temp != 0 && path[temp] != '/'; temp--);
    temp = mx_strlen(&path[temp]);
    while (list_comand) {
        name_comand = list_comand->data;
        creat_list_comands[number_comand++] = mx_strdup(&name_comand[temp - 1]);
        mx_pop_front(&list_comand);
    }
    creat_list_comands[number_comand++] = NULL;
    return creat_list_comands;
}

static void one_comand_in_list(t_info *info, t_list *list_comand, char **str,
                               char *path) {
    int len_comand = mx_strlen(path) - 1;

    for (; len_comand != 0 && path[len_comand] != '/';)
        len_comand--;
        len_comand = mx_strlen(&path[len_comand]);
        path = list_comand->data;
        for (int i = len_comand - 1; path[i]; i++)
            mx_one_symbol(str, path[i], &(MX_STR_LEN), MX_STR_POS);
        if (MX_STR_POS != 0) {
            mx_one_symbol(str, ' ', &(MX_STR_LEN), MX_STR_POS);
            (MX_STR_POS)++;
        }
        mx_pop_front_free_data(&list_comand);
}

char **mx_key_tab(char *parsing, char **str, t_info *info) {
    char *path = mx_mini_parser_tab(parsing, info);
    t_list *list_comand = 0;
    char **creat_list_comand = NULL;
    int comand_len = 0;

    mx_read_comand(path, &list_comand);
    list_comand = mx_sort_list(list_comand, &cmp_str_min_max);
    if (!((comand_len = mx_list_size(list_comand)) == 0 || comand_len == 1))
        creat_list_comand = many_comand_in_list(path, str, info, list_comand);
    else if (comand_len == 1)
        one_comand_in_list(info, list_comand, str, path);
    else
        write(1,"\a", 1);
    mx_strdel(&path);
    mx_strdel(&parsing);
    return creat_list_comand;
}
