#include "ush.h"

void printTokens(t_list *tokens) {
    t_list *tmp = tokens;
    char i = 0;
    char count = 0;
    while(tmp) {
        count++;
        i = 0;
        printf("TOKEN:\n");
        while (((t_token*)tmp->data)->value[i]) {
        printf("value: |%s| type: %d prior: %d\n",((t_token*)tmp->data)->value[i], ((t_token*)tmp->data)->type, ((t_token*)tmp->data)->priority);
        i++;
        }
        tmp = tmp->next;
    }
    if (tokens) {
        printf("\n");
        printf("TOTAL TOKENS: %d\n", count);
    }
}

/* execute all commands ony by one*/
void mx_execute(char **commands) {
    t_list  *tokens  = NULL;
    t_tnode *rootAst = 0;
    int     i        = 0;
    int fds[2];

    while(commands[i]) {
        tokens = mx_lexer(commands[i]);
        if (mx_syntax_analyzer(tokens)) {
            rootAst = mx_create_ast(&tokens, 0);
            pipe(fds);
            mx_execute_tree(rootAst, fds, 0);
            mx_delete_ast(&rootAst);
            close(fds[0]);
            close(fds[1]);
        }
        // printTokens(tokens);
        mx_clear_tokens(&tokens);
        i++;
    }
}