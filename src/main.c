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

    // char *argv2[] = {"4sec",0};
    // pid_t pid = 0;
    // if(fork() == 0) {
    //     if(fork() == 0) {
    //         execv("4sec", argv2);
    //     }
    //     else
    //         execv("10sec", argv2);
    // }
    // else {
    //     wait(0);
    //     printf("DONE!\n");
    // }

    int str = 1;
    t_list *history = NULL;
    char    **commands  = NULL;
    t_info *info = 0;
    t_tnode *change = 0;

    mx_ush_init(&info, envp);
    test();
    while(str != 0) {
        if (history && str == 1) {
            // if (mx_replace_bquote((char**)&(history->data), info)) {
            mx_parsing(history->data, info);
                // commands = mx_create_comands(history->data);
                // commands = mx_strsplit(history->data, ';');
                // mx_execute(commands, info);
                // mx_del_strarr(&commands);
                mx_check_outprogram_new_line();
            // }
        }
        if (info->isExit)
            break;
        str = mx_input(&history);
    }
    // DIR *dir = opendir("..");
    // struct dirent *dn = readdir(dir);
    // while (dn) {
    //     printf("%s\n", dn->d_name);
    // }
    del_list_sring(&history);
    mx_del_strarr(&commands);
    mx_ush_close(info);
    system("leaks ush");
    return info->exit_status;

}

