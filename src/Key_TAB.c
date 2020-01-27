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
    // static char *comand[] = {"./", "/", 0};
    char *temp = 0;
    struct dirent *entry;
    char *directori = direct(parsing);
    int i = 1;
    if (mx_strcmp2("./", directori) == 0)
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
    // for (int i = 0; comand[i]; i++) {
    //     if (mx_strcmp2(comand[i],&parsing[1]) == 0) {
    //         temp = mx_strdup(comand[i]);
    //         mx_push_front(list_comand, temp);
    //     }
    // }
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

    if ((temp > 47 && temp < 58) || (temp > 64 && temp <91))
        return 1;
    if ((temp > 96 && temp < 123) || temp == 46 || temp == '/')
        return 1;
    if (check != 0)
        return 1;
    else
        return 0;
}

static char *mini_parser(char *parsing) {
    int temp;
    char *comands = NULL;

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
    return comands;
}

char **mx_key_tab(char *parsing, int *table, char **str) {
    char *path = mini_parser(parsing);
    t_list *list_comand;
    char **creat_list_comands = NULL;
    int i = 0;
    int temp = 0;

    list_comand = read_comand(path);
    list_comand = mx_sort_list(list_comand, &cmp_str_min_max);
    if (!((temp = mx_list_size(list_comand)) == 0 || temp == 1)) {
        mx_clean_monitor("", table, *str);
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
            mx_one_symbol(str, path[i], &table[2], table[3]);
        if (table[3] != 0) {
            mx_one_symbol(str, ' ', &table[2], table[3]);
            table[3]++;
        }
        mx_pop_front(&list_comand);
    }
    else {
        write(1,"\a", 1);
        free(path);
        free(parsing);
    }
    return creat_list_comands;
}

void mx_key_duble_tab(char **str, char **comands, int *table) {
    if (comands != 0 && comands[0] != 0) {
        if (table[5] == 0 && comands[0] != 0) {
            for (int i = 0; comands[0][i]; i++)
                mx_one_symbol(str, comands[0][i], &table[2], table[3]);
            if (table[3] != 0) {
                mx_one_symbol(str, ' ', &table[2], table[3]);
                table[3]++;
            }
            table[5]++;
        }
        else {
            for (int i = 0; comands[table[5] - 1][i]; i++)
                mx_one_symbol(str, 127, &table[2], table[3]);
            if (comands[table[5]] != 0) {
                for (int i = 0; comands[table[5]][i]; i++)
                    mx_one_symbol(str, comands[table[5]][i], &table[2], table[3]);
                table[5]++;
            }
            else {
                for (int i = 0; comands[0][i]; i++)
                    mx_one_symbol(str, comands[0][i], &table[2], table[3]);
                table[5] = 1;
            }
        }
    }
    else
        write(1,"\a", 1);
}