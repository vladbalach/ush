#include "ush.h"


int mx_pipe_execute(t_tnode *root, int *fds, char operatorStatus, t_info *processes) {
    int fdsBuff[2];
    int newFds[2];
    int status = 0;

    pipe(newFds);
    if ((operatorStatus & 3) == 0) { // PIPE NOTHING
        operatorStatus &= 252;
        fdsBuff[0] = 0;
        fdsBuff[1] = newFds[1];
        if ((operatorStatus & LEFT_VISITED) == 0) {
            operatorStatus &= 63;
            status = mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_PIPE_W, processes);
        }
        fdsBuff[0] = newFds[0];
        fdsBuff[1] = 1;
        if ((operatorStatus & RIGHT_VISITED) == 0) {
            operatorStatus &= 63;
            status = mx_execute_tree(root->right, fdsBuff, operatorStatus | OP_PIPE_R, processes);
        }
    }
    if (operatorStatus & OP_PIPE_R) {
        operatorStatus &= 252;
        fdsBuff[0] = fds[0];
        fdsBuff[1] = newFds[1];
        status = mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_PIPE_RW, processes);
        fdsBuff[0] = newFds[0];
        fdsBuff[1] = 1;
        status = mx_execute_tree(root->right, fdsBuff, operatorStatus | OP_PIPE_R, processes);
    }
    close(newFds[0]);
    close(newFds[1]);
    return status;
}

void mx_execute_proces_(t_token* token) {
    char **argv = 0;
    char *newArg = 0;
    int i = 0;
    int status = 0;

    while (token->value[i]) {
        mx_add_to_strarr(&argv, token->value[i]);
        i++;
    }

    mx_add_to_strarr(&argv, token->value[i]);
    if (execvp(argv[0], argv) == -1) {
        mx_printerr("u$h: command not found: ");
        mx_printerr(token->value[0]);
        mx_printerr("\n");
        mx_del_strarr(&(token->value));
        exit(1);
    }
}

pid_t mx_give_max_index(pid_t *processes) {
    pid_t maxId = 0;

    for(int i = 0; i < MAX_PROC_COUNT; i++)
        if ((maxId < processes[i]) && (getpgid(processes[i]) != -1)) 
            maxId = processes[i];
    return maxId;
}


// static void exec_token_(t_token *token, int *fds, char operatorStatus, t_info *info) {
//     void (*foo)(char *argv[]) = 0;

//     if ((foo = mx_get_buildin(token->value[0]))) { // buildin
//         exec_buidin(token, fds, operatorStatus, foo);
//     }
//     else { // exec program
//         pid_t pid = fork();
//         if (pid == 0) {
//             if (operatorStatus & 7) {
//                 dup2(fds[1],1);
//                 dup2(fds[0],0);
//             }
//             if(mx_get_pr_index(info->processes, pid) != -1) 
//                 mx_execute_proces_(token);
//             else {
//                 printf("Maximum count of background processes - 10\n");
//                 exit(1);
//             }
//         }
//         else {
//             if ((operatorStatus & OP_PIPE_W) || (operatorStatus & OP_MORE))
//                 close(fds[1]);
//             if (operatorStatus & OP_PIPE_R)
//                 close(fds[0]);
//             if (!(operatorStatus & OP_AMPERSAND)) {
//                 wait(0);
//             }
//             else {
//                 if(mx_add_process(info->processes, pid) != -1)
//                     printf("Process [%d] created\n", pid);
//             }
//         }
//     }
// }

int mx_execute_tree(t_tnode *root, int *fds, char operatorStatus, t_info *info) {
    int status = 0;
    char *cmd = 0;
    
    if ((root == 0) || (info->isExit))
        return -1;
    cmd = ((t_token*)root->data)->value[0];

    if (mx_strcmp(((t_token*)root->data)->value[0], "fg") == 0) {
        int maxI = mx_give_max_index(info->processes);
        if (maxI != 0) {
            printf("Get control to %d\n", maxI);
            pid_t ch_pr = 0;
            while (ch_pr != maxI) {
                ch_pr = wait(0);
            }
        }
        else {
            mx_printerr("No fg processes\n");
        }
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], "exit") == 0) {
        int s = atoi(((t_token*)root->data)->value[1]);
        info->isExit = true;
        info->exit_status = s;
        return s;
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], "cd") == 0) {
        mx_cd(((t_token*)root->data)->value, info);
    }
    else if (mx_strcmp(((t_token*)root->data)->value[0], "pwd") == 0) {
        mx_pwd(((t_token*)root->data)->value, info);
    }
    else if (mx_strcmp(((t_token*)root->data)->value[0], "stop") == 0) {
        mx_close_all_pr(info);
    }
    else if (mx_strcmp(((t_token*)root->data)->value[0], "which") == 0) {
        mx_which(((t_token*)root->data)->value, info);
    }
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
