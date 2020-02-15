#include "ush.h"

// static void del_list_sring(t_list **history) {
//     t_list *comand = NULL;

//     if (history != 0 && *history != 0) {
//         comand = *history;
//         while (comand != 0) {
//             free(comand->data);
//             comand = comand->next;
//         }
//     }
// }



// void printStrarr(char **strs) {
//     int i = 0;
    
//     if (strs == 0)
//         return;
//     while(strs[i]) {
//         printf("str[%d]: |%s|\n", i, strs[i]);
//         i++;
//     }
// }

// void free_insert(void *ptr) {
//     free(ptr);
// }

// int cmp(void* a, void* b) {
//     return strcmp(a, b);
// }

// int cmp_del(void* a, void* b) {
//     return strcmp(a, b);
// }

// void free_delete(t_tnode *ptr) {
//     free(ptr->data);
//     free(ptr);
// }


int main(int argc, char *argv[], char **envp) {

    int str = 1;
    t_info *info = 0;
    t_tnode *change = 0;

    mx_ush_init(&info, envp);
    while(str != 0) {
        if (info->history && str == 1) {
            mx_parsing(info->history->data, info);
            mx_check_outprogram_new_line();
        }
        if (info->isExit)
            break;
        str = mx_input(info);
    }
    // mx_ush_close(info);
    system("leaks ush");
    return info->exit_status;
}

// void add(t_info *info) {
//     t_process *pr = (t_process*) malloc (sizeof(t_process));
//     pr->index = 1;
//     pr->pid = 554;

//     mx_push_front(&(info->processes), pr);

    
// }


// int main(int argc, char *argv[], char **envp) {
//     t_info *info;
//     info = (t_info*) malloc (sizeof(t_info));
//     add(info);
//     printf("%d\n", ((t_process*)info->processes->data)->pid);
//     return 0;
// }

