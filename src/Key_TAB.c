#include "ush.h"

static bool cmp_str_min_max(void *str1, void *str2) {
    if (mx_strcmp(str1, str2) > 0)
        return 1;
    else 
        return 0;
}

// static void del_list(t_list **test, char *parsing) {
//     t_list *temp = *test;
//     t_list *temp2 = 0;
//     char *data = temp->data;

//     while (temp != 0 &&mx_strcmp2(data, parsing) != 0) {
//         mx_pop_front(test);
//         temp = *test;
//         if (temp != 0)
//             data = temp->data;
//     }
//     if (temp != 0)
//         temp2 = temp->next;
//     if (temp2 != 0)
//         data = temp2->data;
//     while (temp2 != 0 &&mx_strcmp2(data, parsing) == 0) {
//         temp = temp->next;
//         temp2 = temp2->next;
//         if (temp2 != 0)
//             data = temp2->data;
//     }
//     if (temp != 0)
//         temp->next = 0;
//     while (temp2 != 0)
//         mx_pop_front(&temp2);
// }

static void add_comand(t_list **list_comand, char *parsing) {
    DIR *dir = 0;
    static char *comands[] = {"./", "/", 0};
    char *temp = 0;
    struct dirent *entry;

    if ((dir = opendir("."))) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.' && mx_strcmp2(entry->d_name,parsing) == 0) {
                temp = mx_strdup(entry->d_name);
                mx_push_front(list_comand, temp);
            }
        }
        closedir(dir);
    }
    for (int i = 0; comands[i]; i++) {
        if (mx_strcmp2(comands[i],parsing) == 0) {
            temp = mx_strdup(comands[i]);
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

    for (int i = 0; paths[i]; i++) {
        if ((dir = opendir(paths[i]))) {
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_name[0] != '.' && mx_strcmp2(entry->d_name,parsing) == 0) {
                    path = mx_strdup(entry->d_name);
                    mx_push_front(&list_comand, path);
                }
            }
        }
        closedir(dir);
    }
    add_comand(&list_comand, parsing);
    mx_del_strarr(&paths);
    return list_comand;
}




void mx_key_tab(char *parsing) {
    char *path = NULL;
    t_list *list_comand = read_comand(parsing);
    t_list *test2 = NULL;

    if (parsing == 0)
        parsing = mx_strnew(0);
    list_comand = read_comand(parsing);
    list_comand = mx_sort_list(list_comand, &cmp_str_min_max);
    // del_list(&list_comand, parsing);
    free(parsing);
    test2 = list_comand;
    mx_printstr("\n\n\n");
    for (int i = 0; test2 != 0; i++) {
        path = test2->data;
        mx_printstr(path);
        mx_printstr("  ");
        test2 = test2->next;
    }
    mx_printstr("\n\n\n");
    while (list_comand)
        mx_pop_front(&list_comand);
}
