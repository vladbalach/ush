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

static void del_top_process(t_info *info) {
    if (!info->processes)
        return;
    // free((info->processes)->data);
    mx_pop_front(&info->processes);
}

static int buildin_list(t_token *token, t_info *info) {
    int status = 0;
    if (mx_strcmp(token->value[0], "cd") == 0) 
        status = mx_cd(token->value, info);
    if (mx_strcmp(token->value[0], "pwd") == 0) 
        status = mx_pwd(token->value, info);
    if (mx_strcmp(token->value[0], "echo") == 0) 
        mx_echo(token->value);
    if (mx_strcmp(token->value[0], "exit") == 0) {
        int s = atoi(token->value[1]);
        info->isExit = true;
        info->exit_status = s;
        return s;
    }
    if (mx_strcmp(token->value[0], "fg") == 0) {
        pid_t ch_pr = 0;
        if (info->processes) {
            ch_pr = ((t_process*)info->processes->data)->pid;
            printf("[%d] + continued %d\n",((t_process*)info->processes->data)->index, ch_pr);
            del_top_process(info);
            kill(ch_pr, SIGCONT);
            ch_pr = waitpid(-1, &status, WUNTRACED);
            if (!WIFEXITED(status)) {
                printf("ush: suspended %d\n", ch_pr);
                mx_add_process(&(info->processes), ch_pr);
            }
            else {
                printf("EXIT id = %d\n", ch_pr);
                info->lastStatus = WEXITSTATUS(status);
            }
        }
        else {
            mx_printerr("fg: no current job\n");
        }
    }
    else if (mx_strcmp(token->value[0], "export") == 0)
        mx_export(token->value, &(info->var_tree));
    else if (mx_strcmp(token->value[0], "unset") == 0)
        mx_unset(token->value, &(info->var_tree));
    else if (mx_strcmp(token->value[0], "stop") == 0)
        mx_close_all_pr(info);
    else if (mx_strcmp(token->value[0], "env") == 0)
        mx_env(token->value, info->var_tree);
    else if (mx_strcmp(token->value[0], "which") == 0)
        mx_which(token->value, info);
    return status;
}

static int mx_exec_buildin(t_token *token, int *fds, 
char operator_starus, t_info *info)
{
    int savedFds[2];
    int status = 0;

    set_fds(fds, savedFds);
    status = buildin_list(token, info);
    unset_fds(fds, savedFds, operator_starus);
    return status;
}

int mx_execute_tree(t_tnode *root, int *fds, char operatorStatus, t_info *info) {
    int status = 0;
    char *cmd = 0;
    
    if ((root == 0) || (info->isExit))
        return -1;
    cmd = ((t_token*)root->data)->value[0];
    if (mx_is_buildin(((t_token*)root->data)->value[0])) {
        mx_exec_buildin((t_token*)root->data, fds, operatorStatus, info);
    }
    // if (mx_strcmp(((t_token*)root->data)->value[0], "fg") == 0) {
    //     int maxI = mx_give_max_pr_index(info->processes);
    //     if (maxI != 0) {
    //         printf("Get control to %d\n", maxI);
    //         pid_t ch_pr = 0;
    //         while (ch_pr != maxI) {
    //             ch_pr = wait(0);
    //         }
    //     }
    //     else {
    //         mx_printerr("No fg processes\n");
    //     }
    // }
    // if (mx_strcmp(((t_token*)root->data)->value[0], "exit") == 0) {
    //     int s = atoi(((t_token*)root->data)->value[1]);
    //     info->isExit = true;
    //     info->exit_status = s;
    //     return s;
    // }
    // if (mx_strcmp(((t_token*)root->data)->value[0], "cd") == 0) {
    //     mx_cd(((t_token*)root->data)->value, info);
    // }
    // if (mx_strcmp(((t_token*)root->data)->value[0], "echo") == 0) {
    //     mx_echo(((t_token*)root->data)->value);
    // }
    // else if (mx_strcmp(((t_token*)root->data)->value[0], "export") == 0) {
    //     mx_export(((t_token*)root->data)->value, &(info->var_tree));
    // }
    // else if (mx_strcmp(((t_token*)root->data)->value[0], "unset") == 0) {
    //     mx_unset(((t_token*)root->data)->value, &(info->var_tree));
    // }
    // else if (mx_strcmp(((t_token*)root->data)->value[0], "pwd") == 0) {
    //     mx_pwd(((t_token*)root->data)->value, info);
    // }
    // else if (mx_strcmp(((t_token*)root->data)->value[0], "stop") == 0) {
    //     mx_close_all_pr(info);
    // }
    // else if (mx_strcmp(((t_token*)root->data)->value[0], "env") == 0) {
    //     mx_env(((t_token*)root->data)->value, info->var_tree);
    // }
    // else if (mx_strcmp(((t_token*)root->data)->value[0], "which") == 0) {
    //     mx_which(((t_token*)root->data)->value, info);
    // }
    else if (((t_token*)root->data)->type == TYPE_COMMAND) {
        status = exec_token(root->data, fds, operatorStatus, info);
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], "|") == 0) {
        status = mx_pipe_execute(root, fds, operatorStatus, info);
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], ">") == 0) {
        status = mx_exec_more(root, fds, operatorStatus, info);
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], "<") == 0) {
        mx_exec_less(root, fds, operatorStatus, info);
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], "||") == 0) {
        status = mx_execute_tree(root->left, fds, operatorStatus, info);
        if (status != 0)
            status = mx_execute_tree(root->right, fds, operatorStatus, info);
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], "&&") == 0) {
        status = mx_execute_tree(root->left, fds, operatorStatus, info);
        if (status == 0)
            status = mx_execute_tree(root->right, fds, operatorStatus, info);
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], "&") == 0) {
        exec_token((t_token*)root->left->data, fds, operatorStatus | OP_AMPERSAND, info);
    }
    return status;
}
