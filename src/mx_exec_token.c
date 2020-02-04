#include "ush.h"

static void *mx_get_buildin(char *name) {
    if (mx_strcmp(name, "pwd") == 0)
        return &mx_pwd;
    if (mx_strcmp(name, "cd") == 0)
        return &mx_cd;
    if (mx_strcmp(name, "env") == 0) 
        return &mx_env;
    return 0;
}

static void exec_buidin(t_token *token, int *fds, char operatorStatus, void (*foo)(char *argv[])) {
    pid_t pid = fork();
    if (pid == 0) {
        if (operatorStatus & 3) {
            dup2(fds[1],1);
            dup2(fds[0],0);
        }
        foo(token->value);
        exit(1);
        }
        else {
        if (operatorStatus & OP_PIPE_W)
            close(fds[1]);
        if (operatorStatus & OP_PIPE_R)
            close(fds[0]);
        wait(0);
        }
}

int exec_token(t_token *token, int *fds, char operatorStatus, t_info *info) {
    int status = 0;
    void (*foo)(char *argv[]) = 0;

    if ((foo = mx_get_buildin(token->value[0]))) { // buildin
        exec_buidin(token, fds, operatorStatus, foo);
    }
    else { // exec program
        pid_t pid = fork();
        if (pid == 0) {
            if (operatorStatus & 23) {
                dup2(fds[1],1);
                dup2(fds[0],0);
            }
            if(mx_get_pr_index(info->processes, pid) != -1) 
                mx_execute_proces(token);
            else {
                printf("Maximum count of background processes - 10\n");
                exit(1);
            }
        }
        else {
            if ((operatorStatus & OP_PIPE_W) || (operatorStatus & OP_MORE))
                close(fds[1]);
            if (operatorStatus & OP_PIPE_R)
                close(fds[0]);
            if (!(operatorStatus & OP_AMPERSAND)) {
                wait(&status);
                status = WEXITSTATUS(status);
                if (status == 11)
                    return -1;
            }
            else {
                if(mx_add_process(info->processes, pid) != -1)
                    printf("Process [%d] created\n", pid);
            }
        }
    }
    return 1;
}