#include "ush.h"

static void del_desc(char operatorStatus, int *fds) {
    if ((operatorStatus & OP_PIPE_W) || (operatorStatus & OP_MORE))
        close(fds[1]);
    if (operatorStatus & OP_PIPE_R)
        close(fds[0]);
}

static void child(t_token *token, char operatorStatus, int *fds) {
    if (operatorStatus & 23) {
            dup2(fds[1],1);
            dup2(fds[0],0);
        }
        mx_execute_proces(token);
}

int exec_token(t_token *token, int *fds, char operatorStatus, t_info *info) {
    int exitStatus = 0;
    pid_t pid = fork();

    if (pid == 0)
        child(token, operatorStatus, fds);
    else {
        del_desc(operatorStatus, fds);
        if (!(operatorStatus & OP_AMP)) {
            mx_wait_process(info, token->value);
            return exitStatus;
        }
        else {
            if(mx_add_process(&(info->processes), pid, token->value) != -1)
                printf("Process [%d] created\n", pid);
            printf("pr = %d\n", ((t_process*)info->processes->data)->pid);
        }
    }
    return 1;
}
