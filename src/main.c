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
    // int fds[2];
    // pipe(fds);
    // pid_t pid = fork();

    // if (pid == 0) {
    //     write(fds[1], "Helloow", 8);
    //     write(fds[1], " World", 7);
    //     char buff[100];
    //     int fd = open("1.txt", O_RDWR);
    //     off_t lseek0 = lseek(fds[0], 0, SEEK_CUR);

    //     printf("lseek0 = %lld\n", lseek0);
    //     off_t lseek1 = lseek(0, 0, SEEK_CUR);
    //     printf("lseek1 = %lld\n", lseek1);
    //     // mx_printerr(strerror(errno));
    //     int c = read(fds[0], buff, 2);

    //     // mx_printerr(strerror(errno));
    //     // if(fork() == 0) {
    //     //     char buff[100];
    //     //     int c = read(fds[0], buff, 2);
    //     //     write(1, "1: ",4);
    //     //     write(1, buff, c);
    //     // }
    //     // else 
    //     // {
    //     //     pid = fork();
    //     //     if(pid == 0) {
    //     //         char buff[100];
    //     //         int c = read(fds[0], buff, 2);
    //     //         write(1, "1: ",4);
    //     //         write(1, buff, c);
    //     //     }
    //     //     else if (pid == -1) {
    //     //         printf("Error!\n");
    //     //     }
    //     // }
    // }
    // else {
    //     wait(0);
    // }

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
            commands = mx_strsplit(history->data, ';');
            mx_execute(commands);
            mx_del_strarr(&commands);
        }
        str = mx_input(&history);
        
        
    }
    del_list_sring(&history);
    mx_del_strarr(&commands);
    system("leaks ush");
    return 0;
}

