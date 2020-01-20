#include "ush.h"



void mx_do_actions(char *str) {

}

static void del_list_sring(t_list **history) {
    t_list *comand = NULL;

    if (history != 0 && *history != 0) {
        comand = *history;
        while (comand != 0) {
            free(comand->data);
            comand = comand->next;
        }
    }
}



void printStrarr(char **strs) {
    int i = 0;
    
    if (strs == 0)
        return;
    while(strs[i]) {
        printf("str[%d]: |%s|\n", i, strs[i]);
        i++;
    }
}



int main(int argc, char *argv[], char *envp[]) {
    bool    str         = 1;
    t_list  *history    = NULL;
    char    **commands  = NULL;
    int     i = 0;

    mx_ush_init();
    while(str) {
        if (history) {
            mx_parsing(history->data);
            mx_do_actions(history->data);
        }
        str = mx_input(&history);
        // char *str2 = mx_strdup("cat -e | cat");
        commands = mx_strsplit(history->data, ';');
        mx_execute(commands);
        mx_del_strarr(&commands);
        // printStrarr(commands);
        // printTokens(tokens);
    }
    del_list_sring(&history);

    system("leaks ush");
    return 0;
}

