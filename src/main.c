#include "ush.h"

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

unsigned int mx_mix_atoi(char *s){
    unsigned int a = 0;
    for(int i = 0; i < mx_strlen(s); i++){
        a = a * 10 + (s[i] - '0');
    }
    return a;
}

static void test() {
    char *str = getenv("SHLVL");
    int i = 0;
    char *temp = 0;

    i = mx_mix_atoi(str);
    temp = mx_itoa(++i);
    str = mx_strjoin("SHLVL=", temp);
    putenv(str);
    free(temp);
}

void free_insert(void *ptr) {
    free(ptr);
}

int cmp(void* a, void* b) {
    return strcmp(a, b);
}

int cmp_del(void* a, void* b) {
    return strcmp(a, b);
}

void free_delete(t_tnode *ptr) {
    free(ptr->data);
    free(ptr);
}

int main(int argc, char *argv[], char **envp) {
    bool str = 1;
    t_list *history = NULL;
    t_list *tokens = 0;
    char    **commands  = NULL;
    int     i = 0;

    mx_ush_init();

    test();
    while(str) {
        if (history) {
            mx_parsing(history->data, envp);
        }
        str = mx_input(&history);
        commands = mx_strsplit(history->data, ';');
        mx_execute(commands);
        mx_del_strarr(&commands);
    }
    del_list_sring(&history);
    mx_del_strarr(&commands);
    system("leaks ush");
    return 0;
}

