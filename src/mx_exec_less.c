#include "ush.h"

static void operator_0(t_tnode *root, int fd, char operatorStatus,
                       t_info *info) {
    int newFds[2];

    newFds[0] = fd;
    newFds[1] = 1;
    exec_token((t_token*)root->left->data, newFds,
                operatorStatus | OP_LESS, info);
}

static void close_fds(int *newFds) {
    close(newFds[0]);
    close(newFds[0]);
}

static void set_new_fds(int *new_fds, int fd0, int fd1) {
    new_fds[0] = fd0;
    new_fds[1] = fd1;
}

void mx_exec_less(t_tnode *root, int *fds, char operatorStatus,
                  t_info *info) {
    int fd = open(((t_token*)root->right->data)->value[0], O_RDWR);
    int newFds[2];

    if (operatorStatus == 0)
        operator_0(root, fd, operatorStatus, info);
    if (operatorStatus & OP_PIPE_W) {
        set_new_fds(newFds, fd, fds[1]);
        exec_token((t_token*)root->left->data, newFds,
                   operatorStatus | OP_LESS, info);
    }
    if (operatorStatus & OP_PIPE_R) {
        set_new_fds(newFds, fds[0], 1);
        exec_token((t_token*)root->left->data, newFds,
                   operatorStatus | OP_LESS, info);
    }
    if (operatorStatus & OP_PIPE_RW) {
        set_new_fds(newFds, fds[0], fds[1]);
        exec_token((t_token*)root->left->data, newFds,
                   operatorStatus | OP_LESS, info);
    }
    close_fds(newFds);
}
