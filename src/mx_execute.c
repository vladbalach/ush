#include "ush.h"

/* execute all commands ony by one*/
void mx_execute(char **commands, t_info *processes) {
    t_list  *tokens  = NULL;
    t_tnode *root_ast = 0;
    int i = 0;

    while (commands[i]) {
        tokens = mx_lexer(commands[i], processes);
        if (processes->if_ctrl_c && mx_syntax_analyzer(tokens)) {
            root_ast = mx_create_ast(&tokens, 0);
            mx_execute_tree(root_ast, 0, 0, processes);
            mx_delete_ast(&root_ast);
        }
        else
            mx_clear_tokens(&tokens);
        if (!(processes->if_ctrl_c))
            while (commands[++i]);
        else
            i++;
    }
}
