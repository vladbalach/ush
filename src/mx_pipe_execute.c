#include "ush.h"

static void close_desc(int *fds) {
    close(fds[0]);
    close(fds[1]);
}

static int pipe_r(t_tnode *root, int *fds, char operatorStatus,
                  t_info *processes) {
    int newFds[2];
    int fdsBuff[2];
    int status = 0;

    pipe(newFds);
    operatorStatus &= 252;
    fdsBuff[0] = fds[0];
    fdsBuff[1] = newFds[1];
    status = mx_execute_tree(root->left, fdsBuff,
                             operatorStatus | OP_PIPE_RW, processes);
    fdsBuff[0] = newFds[0];
    fdsBuff[1] = 1;
    status = mx_execute_tree(root->right, fdsBuff,
                             operatorStatus | OP_PIPE_R, processes);
    close_desc(newFds);
    return status;
}

static int no_pipe(t_tnode *root, char opst, t_info *pr) {
    int newFds[2];
    int fdsBuff[2];
    int status = 0;

    pipe(newFds);
    opst &= 252;
    fdsBuff[0] = 0;
    fdsBuff[1] = newFds[1];
    if ((opst & LEFT_VISITED) == 0) {
        opst &= 63;
        status = mx_execute_tree(root->left, fdsBuff, opst | OP_PIPE_W, pr);
    }
    fdsBuff[0] = newFds[0];
    fdsBuff[1] = 1;
    if ((opst & RIGHT_VISITED) == 0) {
        opst &= 63;
        status = mx_execute_tree(root->right, fdsBuff, opst | OP_PIPE_R, pr);
    }
    close_desc(newFds);
    return status;
}



int mx_pipe_execute(t_tnode *root, int *fds, char operatorStatus,
                    t_info *processes) {
    int status = 0;

    if ((operatorStatus & 3) == 0) // PIPE NOTHING
        status = no_pipe(root, operatorStatus, processes);
    if (operatorStatus & OP_PIPE_R)
        status = pipe_r(root, fds, operatorStatus, processes);
    return status;
}
