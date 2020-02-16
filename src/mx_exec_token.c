#include "ush.h"

static void wait_process(t_info *info, t_token *token) {
    int status = 0;
    pid_t pr = 0;
    char **mas_name = 0;

    pr = waitpid(-1, &status, WUNTRACED); 
    if (!WIFEXITED(status)) {
        mx_add_process(&(info->processes), pr, token->value);
        mas_name = mx_get_name(info, pr);
        mx_print_susp(mas_name);
    }
    else {
        mx_del_top_process(info);
        info->lastStatus = WEXITSTATUS(status);
    }
}

static void del_desc(char operatorStatus, int *fds) {
    if ((operatorStatus & OP_PIPE_W) || (operatorStatus & OP_MORE))
        close(fds[1]);
    if (operatorStatus & OP_PIPE_R)
        close(fds[0]);
}

int exec_token(t_token *token, int *fds, char operatorStatus, t_info *info) {
    int exitStatus = 0;
    pid_t pid = fork();

    if (pid == 0) {
        if (operatorStatus & 23) {
            dup2(fds[1],1);
            dup2(fds[0],0);
        }
        mx_execute_proces(token);
    }
    else {
        del_desc(operatorStatus, fds);
        if (!(operatorStatus & OP_AMPERSAND)) {
            wait_process(info, token);
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