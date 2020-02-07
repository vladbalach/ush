#include "ush.h"

void mx_exec_less(t_tnode *root, int *fds, char operatorStatus, t_info *info) {
    int fd = open(((t_token*)root->right->data)->value[0], O_RDWR);
    int newFds[2];
    if (operatorStatus == 0) {
        newFds[0] = fd;
        newFds[1] = 1;
        exec_token((t_token*)root->left->data, newFds, operatorStatus | OP_LESS, info);
    }
    if (operatorStatus & OP_PIPE_W) {
        newFds[0] = fd;
        newFds[1] = fds[1];
        exec_token((t_token*)root->left->data, newFds, operatorStatus | OP_LESS, info);
    }
    if (operatorStatus & OP_PIPE_R) {
        newFds[0] = fds[0];
        newFds[1] = 1;
        exec_token((t_token*)root->left->data, newFds, operatorStatus | OP_LESS, info);
    }
    if (operatorStatus & OP_PIPE_RW) {
        newFds[0] = fds[0];
        newFds[1] = fds[1];
        exec_token((t_token*)root->left->data, newFds, operatorStatus | OP_LESS, info);
    }

    close(newFds[0]);
    close(newFds[0]);
}
