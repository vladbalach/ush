#include "ush.h"

static void close_desc(int *fds) {
    close(fds[0]);
    close(fds[1]);
}

static int pipe_r(t_tnode *root, int *fds, char operator_status,
                  t_info *processes) {
    int new_fds[2];
    int fds_buff[2];
    int status = 0;

    pipe(new_fds);
    operator_status &= 252;
    fds_buff[0] = fds[0];
    fds_buff[1] = new_fds[1];
    status = mx_execute_tree(root->left, fds_buff,
                             operator_status | OP_PIPE_RW, processes);
    fds_buff[0] = new_fds[0];
    fds_buff[1] = 1;
    status = mx_execute_tree(root->right, fds_buff,
                             operator_status | OP_PIPE_R, processes);
    close_desc(new_fds);
    return status;
}

static int no_pipe(t_tnode *root, char opst, t_info *pr) {
    int new_fds[2];
    int fds_buff[2];
    int status = 0;

    pipe(new_fds);
    opst &= 252;
    fds_buff[0] = 0;
    fds_buff[1] = new_fds[1];
    if ((opst & LEFT_VISITED) == 0) {
        opst &= 63;
        status = mx_execute_tree(root->left, fds_buff, opst | OP_PIPE_W, pr);
    }
    fds_buff[0] = new_fds[0];
    fds_buff[1] = 1;
    if ((opst & RIGHT_VISITED) == 0) {
        opst &= 63;
        status = mx_execute_tree(root->right, fds_buff, opst | OP_PIPE_R, pr);
    }
    close_desc(new_fds);
    return status;
}



int mx_pipe_execute(t_tnode *root, int *fds, char operator_status,
                    t_info *processes) {
    int status = 0;

    if ((operator_status & 3) == 0) // PIPE NOTHING
        status = no_pipe(root, operator_status, processes);
    if (operator_status & OP_PIPE_R)
        status = pipe_r(root, fds, operator_status, processes);
    return status;
}
