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

void printTokens(t_list *tokens) {
    t_list *tmp = tokens;
    while(tmp) {
        printf("%s\n",((t_token*)tmp->data)->value);
        tmp = tmp->next;
    }
    printf("\n\n");
}

int main(int argc, char *argv[], char *envp[]) {
    bool str = 1;
    t_list *history = NULL;
    t_list *tokens = 0;
    while(str) {
        if (history) {
            mx_parsing(history->data);
            mx_do_actions(history->data);
        }
        str = mx_input(&history);
        char *str2 = mx_strdup("cat -e | cat");
        tokens = mx_lexer(str2);
        printTokens(tokens);
        mx_clear_tokens(&tokens);
    }

    del_list_sring(&history);

    system("leaks ush");
    return 0;
}

