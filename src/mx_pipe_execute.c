#include "ush.h"

int mx_pipe_execute(t_tnode *root, int *fds, char operatorStatus, t_info *processes) {
    int fdsBuff[2];
    int newFds[2];
    int status = 0;

    pipe(newFds);
    if ((operatorStatus & 3) == 0) { // PIPE NOTHING
        operatorStatus &= 252;
        fdsBuff[0] = 0;
        fdsBuff[1] = newFds[1];
        if ((operatorStatus & LEFT_VISITED) == 0) {
            operatorStatus &= 63;
            status = mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_PIPE_W, processes);
        }
        fdsBuff[0] = newFds[0];
        fdsBuff[1] = 1;
        if ((operatorStatus & RIGHT_VISITED) == 0) {
            operatorStatus &= 63;
            status = mx_execute_tree(root->right, fdsBuff, operatorStatus | OP_PIPE_R, processes);
        }
    }
    if (operatorStatus & OP_PIPE_R) {
        operatorStatus &= 252;
        fdsBuff[0] = fds[0];
        fdsBuff[1] = newFds[1];
        status = mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_PIPE_RW, processes);
        fdsBuff[0] = newFds[0];
        fdsBuff[1] = 1;
        status = mx_execute_tree(root->right, fdsBuff, operatorStatus | OP_PIPE_R, processes);
    }
    close(newFds[0]);
    close(newFds[1]);
    return status;
}
