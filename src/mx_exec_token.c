#include "ush.h"

int exec_token(t_token *token, int *fds, char operatorStatus, t_info *info) {
    int status = 0;
    int exitStatus = 0;
    pid_t pr = 0;
    pid_t pid = fork();
    if (pid == 0) {
        if (operatorStatus & 23) {
            dup2(fds[1],1);
            dup2(fds[0],0);
        }
        mx_execute_proces(token);
    }
    else {
        if ((operatorStatus & OP_PIPE_W) || (operatorStatus & OP_MORE))
            close(fds[1]);
        if (operatorStatus & OP_PIPE_R)
            close(fds[0]);
        if (!(operatorStatus & OP_AMPERSAND)) {
            pr = waitpid(-1, &status, WUNTRACED);
            if (!WIFEXITED(status)) {
                printf("STOPPED\n");
                mx_add_process(&(info->processes), pr);
            }
            else {
                info->lastStatus = WEXITSTATUS(status);
            }
            return exitStatus;
        }
        else {
            if(mx_add_process(&(info->processes), pid) != -1)
                printf("Process [%d] created\n", pid);
            printf("pr = %d\n", ((t_process*)info->processes->data)->pid);
        }
    }
    return 1;
}
