#include "ush.h"

static void set_fds(int *fds, int *savedFds) {
    savedFds[0] = dup(0);
    savedFds[1] = dup(1);
    if (fds) {
        dup2(fds[0], 0);
        dup2(fds[1], 1);
    }
}

static int mx_exec_buildin(t_token *token, int *fds,
                           char operator_starus, t_info *info) {
    int savedFds[2];
    int status = 0;

    set_fds(fds, savedFds);
    status = mx_buildin_list(token, info);
    mx_unset_fds(fds, savedFds, operator_starus);
    return status;
}

static int or_operator(t_tnode *root, int *fds, char operatorStatus,
                       t_info *info) {
    int status = 0;

    status = mx_execute_tree(root->left, fds, operatorStatus, info);
    if (info->lastStatus != 0)
        status = mx_execute_tree(root->right, fds, operatorStatus, info);
    return status;
}

static int and_operator(t_tnode *root, int *fds, char operatorStatus,
                        t_info *info) {
    int status = 0;

    status = mx_execute_tree(root->left, fds, operatorStatus, info);
    if (info->lastStatus == 0)
        status = mx_execute_tree(root->right, fds, operatorStatus, info);
    return status;
}

int mx_execute_tree(t_tnode *root, int *fds, char op_st, t_info *info) {
    int status = 0;

    if ((root == 0) || (info->isExit))
        return -1;
    if (mx_is_buildin(((t_token*)root->data)->value[0]))
        mx_exec_buildin((t_token*)root->data, fds, op_st, info);
    else if (((t_token*)root->data)->type == TYPE_COMMAND)
        status = exec_token(root->data, fds, op_st, info);
    if (mx_strcmp(((t_token*)root->data)->value[0], "|") == 0)
        status = mx_pipe_execute(root, fds, op_st, info);
    if (mx_strcmp(((t_token*)root->data)->value[0], ">") == 0)
        status = mx_exec_more(root, fds, op_st, info);
    if (mx_strcmp(((t_token*)root->data)->value[0], "<") == 0)
        mx_exec_less(root, fds, op_st, info);
    if (mx_strcmp(((t_token*)root->data)->value[0], "||") == 0)
        status = or_operator(root, fds, op_st, info);
    if (mx_strcmp(((t_token*)root->data)->value[0], "&&") == 0)
        status = and_operator(root, fds, op_st, info);
    return status;
}
