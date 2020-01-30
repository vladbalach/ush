#include "ush.h"

void mx_exec_less(t_tnode *root, int *fds, char operatorStatus, t_info *info) {

    int fd = open(((t_token*)root->right->data)->value[0], O_CREAT, S_IWRITE | S_IREAD);
    pid_t pid = fork();
    if (pid == 0) {
        if (operatorStatus & OP_PIPE_W) {
            dup2(fd, 0);
            dup2(fds[1], 1);
            
        }
        if (operatorStatus & OP_PIPE_R) {
            dup2(fds[0], 0);
            mx_write_from_to(fd, fds[1], 0);
            close(fd);
        }
        if ((operatorStatus & 192) == 0) {
            dup2(fd,0);
        }
        close(fds[1]);
        close(fds[0]);
        mx_execute_proces((t_token*)root->left->data);
    }
    else {
        close(fd);
    }

}
