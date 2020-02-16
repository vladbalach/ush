#include "ush.h"

void set_fds(int *fds, int *savedFds) {
    savedFds[0] = dup(0);
    savedFds[1] = dup(1);
    if (fds) {
        // printf("fds[0] = %d\nfds[1] = %d\n", fds[0], fds[1]);
        dup2(fds[0], 0);
        dup2(fds[1], 1);
    }
}

void unset_fds(int *fds, int *savedFds, int operator_starus) {
    dup2(savedFds[0], 0);
    dup2(savedFds[1], 1);
    if ((operator_starus & OP_PIPE_W) || (operator_starus & OP_MORE))
        close(fds[1]);
    if (operator_starus & OP_PIPE_R)
        close(fds[0]);
    close(savedFds[0]);
    close(savedFds[1]);
}

static int mx_exec_buildin(t_token *token, int *fds, 
char operator_starus, t_info *info)
{
    int savedFds[2];
    int status = 0;

    set_fds(fds, savedFds);
    status = mx_buildin_list(token, info);
    unset_fds(fds, savedFds, operator_starus);
    return status;
}

int mx_execute_tree(t_tnode *root, int *fds, char operatorStatus, t_info *info) {
    int status = 0;
    char *cmd = 0;
    
    if ((root == 0) || (info->isExit))
        return -1;
    cmd = ((t_token*)root->data)->value[0];
    if (mx_is_buildin(((t_token*)root->data)->value[0]))
        mx_exec_buildin((t_token*)root->data, fds, operatorStatus, info);
    else if (((t_token*)root->data)->type == TYPE_COMMAND)
        status = exec_token(root->data, fds, operatorStatus, info);
    if (mx_strcmp(((t_token*)root->data)->value[0], "|") == 0)
        status = mx_pipe_execute(root, fds, operatorStatus, info);
    if (mx_strcmp(((t_token*)root->data)->value[0], ">") == 0)
        status = mx_exec_more(root, fds, operatorStatus, info);
    if (mx_strcmp(((t_token*)root->data)->value[0], "<") == 0)
        mx_exec_less(root, fds, operatorStatus, info);
    if (mx_strcmp(((t_token*)root->data)->value[0], "||") == 0) {
        status = mx_execute_tree(root->left, fds, operatorStatus, info);
        if (info->lastStatus != 0)
            status = mx_execute_tree(root->right, fds, operatorStatus, info);
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], "&&") == 0) {
        status = mx_execute_tree(root->left, fds, operatorStatus, info);
        if (info->lastStatus == 0)
            status = mx_execute_tree(root->right, fds, operatorStatus, info);
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], "&") == 0)
        exec_token((t_token*)root->left->data, fds, operatorStatus | OP_AMPERSAND, info);
    return status;
}
