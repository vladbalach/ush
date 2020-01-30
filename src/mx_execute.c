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

    while(commands[i]) {
        tokens = mx_lexer(commands[i]);
        if (mx_syntax_analyzer(tokens)) {
            rootAst = mx_create_ast(&tokens, 0);
            mx_execute_tree(rootAst, 0, 0);
            mx_delete_ast(&rootAst);
        }
        printTokens(tokens);
        mx_clear_tokens(&tokens);
        i++;
    }
}