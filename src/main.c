#include "ush.h"
// signals 
// void out_monitor2(int position, char *str, int count, int ch);
// void mx_init(t_info* mainInfo) {
//     signal(SIGINT, mx_sig_int);
//     signal(SIGKILL, mx_sig_kill);
//     // set parametrs for terminal
//     setTerminal(mainInfo);
// }

// void mx_close(t_info *mainInfo, char **str) {
//     free(*str);
//     tcsetattr(STDIN_FILENO, TCSAFLUSH, &mainInfo->term_old);
// }

// // input
// // 1 - end of input ( \n ; )

void mx_parsing(char *str) {
    mx_printstr("str = ");
    mx_printstr(str);
    mx_printstr("|\n");
}

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

int main(int argc, char *argv[], char *envp[]) {
    bool str = 1;
    t_list *history = NULL;

    while(str) {
        if (history) {
            mx_parsing(history->data);
            mx_do_actions(history->data);
        }
        str = mx_input(&history);
    }
    del_list_sring(&history);
    system("leaks ush");
    return 0;
}

