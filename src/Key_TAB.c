#include "ush.h"

static bool cmp_str_min_max(void *str1, void *str2) {
    if (mx_strcmp(str1, str2) > 0)
        return 1;
    else 
        return 0;
}

static bool comands(char temp) {
    if (temp == '\r'|| temp == '|' || temp == '&')
        return 1;
    else
        return 0;
}

static char *direct(char *parsing) {
    int i = mx_strlen(parsing);

    if (comands(parsing[0]) == 0) {
        while(i != 0 && parsing[--i] != '/');
        if (i == 0)
            return mx_strdup("./");
        else
            return mx_strndup(&parsing[1], i);
    }
    return mx_strdup("./");
}

static void add_comand(t_list **list_comand, char *parsing) {
    DIR *dir = 0;
    static char *comand[] = {"history", "./", "../", 0};
    char *temp = 0;
    struct dirent *entry;
    char *directori = direct(parsing);
    int i = 1;

    if (mx_strcmp2("./", directori) == 0 && mx_strcmp2(&parsing[1], "./") != 0)
        i = 1;
    else
        i = mx_strlen(directori) + 1;
    if ((comands(parsing[0]) == 0) && (dir = opendir(directori))) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.' && mx_strcmp2(entry->d_name, &parsing[i]) == 0) {
                temp = mx_strdup(entry->d_name);
                mx_push_front(list_comand, temp);
            }
        }
        closedir(dir);
    }
    free(directori);
    for (i = 0; comands(parsing[0]) != 0 && comand[i]; i++) {
        if (mx_strcmp2(comand[i],&parsing[1]) == 0) {
            temp = mx_strdup(comand[i]);
            mx_push_front(list_comand, temp);
        }
    }
}




static t_list *read_comand(char *parsing) {
    char *path = getenv("PATH");
    char **paths = mx_strsplit(path, ':');
    DIR *dir = 0;
    struct dirent *entry;
    t_list *list_comand = NULL;

    if (comands(parsing[0])) {
        for (int i = 0; paths[i]; i++) {
            if ((dir = opendir(paths[i]))) {
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] != '.' && mx_strcmp2(entry->d_name, &parsing[1]) == 0) {
                        path = mx_strdup(entry->d_name);
                        mx_push_front(&list_comand, path);
                    }
                }
                closedir(dir);
            }
        }
    }
    add_comand(&list_comand, parsing);
    mx_del_strarr(&paths);
    return list_comand;
}

static bool name_comand(char temp) {
    char check = temp >> 7;

    if ((temp > 47 && temp < 58) || (temp > 64 && temp <91) || temp == '~' || temp == '_')
        return 1;
    if ((temp > 96 && temp < 123) || temp == 46 || temp == '/' || temp == '.' || temp == '+' || temp == '-')
        return 1;
    if (check != 0)
        return 1;
    else
        return 0;
}

static char *mini_parser(char *parsing, t_info *info) {
    int temp;
    char *comands = NULL;
    char *test;
    char a;

    if (parsing == 0)
        return mx_strnew(0);
    temp = mx_strlen(parsing) - 1;
    while (temp > -1 && name_comand(parsing[temp]))
        temp--;
    if (temp == -1)
        comands =  mx_strjoin("\r", parsing);
    else
        comands = mx_strdup(&parsing[temp]);
    while (temp > -1 && parsing[temp] == ' ')
        temp--;
    if (temp == -1)
        comands[0] = '\r';
    else
        comands[0] = parsing[temp];
    if (mx_strstr(comands, "/"))
        comands[0] = '1';
    if (comands[1] == '~') {
        temp = 0;
        test = mx_strdup(&comands[1]);
        a = comands[0];
        mx_strdel(&comands);
        mx_HOME(&test, &temp, info);
        comands = mx_strjoin("\r", test);
        mx_strdel(&test);
        comands[0] = a;
    }
    return comands;
}

char **mx_key_tab(char *parsing, char **str, t_info *info) {
    char *path = mini_parser(parsing, info);
    t_list *list_comand;
    char **creat_list_comands = NULL;
    int i = 0;
    int temp = 0;

    list_comand = read_comand(path);
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

void mx_key_duble_tab(char **str, char **comands, t_info *info) {
    if (comands != 0 && comands[0] != 0) {
        if (MX_ID_TAB_KEY == 0 && comands[0] != 0) {
            for (int i = 0; comands[0][i]; i++)
                mx_one_symbol(str, comands[0][i], &(MX_STR_LEN), MX_STR_POS);
            if (MX_STR_POS != 0) {
                mx_one_symbol(str, ' ', &(MX_STR_LEN), MX_STR_POS);
                MX_STR_POS++;
            }
            MX_ID_TAB_KEY++;
        }
        else {
            for (int i = 0; comands[MX_ID_TAB_KEY - 1][i]; i++)
                mx_one_symbol(str, 127, &(MX_STR_LEN), MX_STR_POS);
            if (comands[MX_ID_TAB_KEY] != 0) {
                for (int i = 0; comands[MX_ID_TAB_KEY][i]; i++)
                    mx_one_symbol(str, comands[MX_ID_TAB_KEY][i], &(MX_STR_LEN), MX_STR_POS);
                MX_ID_TAB_KEY++;
            }
            else {
                for (int i = 0; comands[0][i]; i++)
                    mx_one_symbol(str, comands[0][i], &(MX_STR_LEN), MX_STR_POS);
                MX_ID_TAB_KEY = 1;
            }
        }
    }
    else
        write(1,"\a", 1);
}