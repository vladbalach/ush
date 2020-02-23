#include "ush.h"

void mx_unset_fds(int *fds, int *saved_fds, int operator_starus) {
    dup2(saved_fds[0], 0);
    dup2(saved_fds[1], 1);
    if ((operator_starus & OP_PIPE_W) || (operator_starus & OP_MORE))
        close(fds[1]);
    if (operator_starus & OP_PIPE_R)
        close(fds[0]);
    close(saved_fds[0]);
    close(saved_fds[1]);
}
