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

static int pipe_w(t_tnode *root, int *fds, int operator_status, t_info *info) {
    int fdsBuff[2];
    int status = 0;
    int fd = open(((t_token*)(root->right->data))->value[0], MX_FLAGS_W);

    if (print_error(fd))
        return -1;
    fdsBuff[0] = fds[0];
    fdsBuff[1] = fd;
    status = mx_execute_tree(root->left, fdsBuff, operator_status
                             | OP_MORE, info);
    close(fds[1]);
    fd = open(((t_token*)(root->right->data))->value[0],
              O_RDWR , S_IWRITE | S_IREAD);
    fds[1] = fd;
    status = mx_execute_tree(root->left, fdsBuff, operator_status
                             | OP_MORE | LEFT_VISITED, info);
    close(fd);
    return status;
}

static int pipe_r(t_tnode *root, int *fds, int operator_status, t_info *info) {
    int fdsBuff[2];
    int status = 0;
    int fd = 0;

    fd = open(((t_token*)(root->right->data))->value[0], MX_FLAGS_W);
    if (print_error(fd))
        return 1;
    fdsBuff[0] = fds[0];
    fdsBuff[1] = fd;
    status = mx_execute_tree(root->left, fdsBuff,
                             operator_status | OP_MORE, info);
    return status;
}

int mx_exec_more(t_tnode *root, int *fds, int operator_status, t_info *info) {
    int fdsBuff[2];
    int status = 0;
    int fd = 0;

    if ((operator_status & 3) == 0) {
        fd = open(((t_token*)(root->right->data))->value[0], MX_FLAGS_W);
        if (print_error(fd))
            return -1;
        fdsBuff[0] = 0;
        fdsBuff[1] = fd;
        status = mx_execute_tree(root->left, fdsBuff,
                                 operator_status | OP_MORE, info);
    }
    if (operator_status & OP_PIPE_W)
        status = pipe_w(root, fds, operator_status, info);
    if (operator_status & OP_PIPE_R)
        status = pipe_r(root, fds, operator_status, info);
    return status;
}
