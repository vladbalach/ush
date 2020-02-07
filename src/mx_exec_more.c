#include "ush.h"

static bool print_error(int fd) {
    if (fd == -1) {
        mx_printerr("ush: ");
        mx_printerr(strerror(errno));
        mx_printerr("\n");
        return true;
    }
    return false;
}

int mx_exec_more(t_tnode *root, int *fds, int operatorStatus, t_info *info) {
    int fdsBuff[2];
    int status = 0;

    if ((operatorStatus & 3) == 0) {
        int fd = open(((t_token*)(root->right->data))->value[0], O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
        if (print_error(fd))
            return -1;
        fdsBuff[0] = 0;
        fdsBuff[1] = fd;
        status = mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_MORE, info);
    }
    if (operatorStatus & OP_PIPE_W) {
        int fd = open(((t_token*)(root->right->data))->value[0], O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
        if (print_error(fd))
            return -1;
        fdsBuff[0] = fds[0];
        fdsBuff[1] = fd;
        status = mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_MORE, info);
        close(fds[1]);
        fd = open(((t_token*)(root->right->data))->value[0], O_RDWR , S_IWRITE | S_IREAD);
        fds[1] = fd;
        status = mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_MORE | LEFT_VISITED, info);
        close(fd);
    }
    if (operatorStatus & OP_PIPE_R) {
        int fd = open(((t_token*)(root->right->data))->value[0], O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
        if (print_error(fd))
            return -1;
        fdsBuff[0] = fds[0];
        fdsBuff[1] = fd;
        status = mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_MORE, info);
    }
    return status;
}
