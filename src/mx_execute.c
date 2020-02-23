#include "ush.h"

/* execute all commands ony by one*/
void mx_execute(char **commands, t_info *processes) {
    t_list  *tokens  = NULL;
    t_tnode *rootAst = 0;
    int i = 0;

    while (commands[i]) {
        tokens = mx_lexer(commands[i], processes);
        if (processes->lastStatus != 130 && mx_syntax_analyzer(tokens)) {
            rootAst = mx_create_ast(&tokens, 0);
            mx_execute_tree(rootAst, 0, 0, processes);
            mx_delete_ast(&rootAst);
        }
        else
            mx_clear_tokens(&tokens);
        if (processes->lastStatus == 130)
            while (commands[++i]);
        else
            i++;
    }
}
