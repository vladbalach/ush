#include "ush.h"


void mx_pipe_execute(t_tnode *root, int *fds, char operatorStatus) {
    int fdsBuff[2];
    int newFds[2];
    pipe(newFds);
    if ((operatorStatus & 3) == 0) { // PIPE NOTHING
        operatorStatus &= 252;
        fdsBuff[0] = 0;
        fdsBuff[1] = newFds[1];
        if ((operatorStatus & LEFT_VISITED) == 0) {
            operatorStatus &= 63;
            mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_PIPE_W);
        }
        fdsBuff[0] = newFds[0];
        fdsBuff[1] = 1;
        if ((operatorStatus & RIGHT_VISITED) == 0) {
            operatorStatus &= 63;
            mx_execute_tree(root->right, fdsBuff, operatorStatus | OP_PIPE_R);
        }
    }
    if (operatorStatus & OP_PIPE_R) {
        operatorStatus &= 252;
        fdsBuff[0] = fds[0];
        fdsBuff[1] = newFds[1];
        mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_PIPE_RW);
        fdsBuff[0] = newFds[0];
        fdsBuff[1] = 1;
        mx_execute_tree(root->right, fdsBuff, operatorStatus | OP_PIPE_R);
    }
    close(newFds[0]);
    close(newFds[1]);
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

void *mx_get_buildin(char *name) {
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

static void exec_token_(t_token *token, int *fds, char operatorStatus) {
    void (*foo)(char *argv[]) = 0;

    if ((foo = mx_get_buildin(token->value[0]))) { // buildin
        exec_buidin(token, fds, operatorStatus, foo);
    }
    else { // exec program
        pid_t pid = fork();
        if (pid == 0) {
        if (operatorStatus & 7) {
            dup2(fds[1],1);
            dup2(fds[0],0);
        }
        mx_execute_proces_(token);
        }
        else {
        if ((operatorStatus & OP_PIPE_W) || (operatorStatus & OP_MORE))
            close(fds[1]);
        if (operatorStatus & OP_PIPE_R)
            close(fds[0]);
        wait(0);
        }
    }
}

void mx_execute_tree(t_tnode *root, int *fds, char operatorStatus) {
    if (root == 0)
        return;
    char *cmd = ((t_token*)root->data)->value[0];
    if (((t_token*)root->data)->type == TYPE_COMMAND) {
        exec_token_(root->data, fds, operatorStatus);
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], "|") == 0) {
        mx_pipe_execute(root, fds, operatorStatus);
    }
    if (mx_strcmp(((t_token*)root->data)->value[0], ">") == 0) {
        mx_exec_more(root, fds, operatorStatus);
    }
}
