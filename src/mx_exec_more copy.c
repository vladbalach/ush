#include "ush.h"

static int setSeek(char operatorStatus, t_token *token, off_t *start) {
    int fd = 0;
    if (operatorStatus & OP_MORE) {
        fd = open(token->value[0], 
            O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
        *start = lseek(fd, 0, SEEK_SET);
    }
    else {
        fd = open(token->value[0], O_RDWR | O_CREAT, S_IWRITE | S_IREAD);
        *start = lseek(fd, 0, SEEK_END);
    }
    return fd;
}

static void exec_token_more(t_tnode *root, int *fds, char operatorStatus, int fd) {
    dup2(fd, 1);
    if (operatorStatus & OP_PIPE_R)
        dup2(fds[0],0);
    close(fds[1]);
    close(fds[0]);
    mx_execute_proces((t_token*) root->data);
}

static bool is_less(t_tnode *root) {
    if (((t_token*)root->left->data)->type == TYPE_OPERATOR) { // < 
        return true;
    }
    return false;
}

static void less_false(t_tnode *root, int *fds, int operatorStatus) {
    int     fd      = 0;
    off_t start = 0;
    pid_t pid   = 0;

    fd = setSeek(operatorStatus, (t_token*)root->right->data, &start);
    pid = fork();
    if (pid == 0) {    // child
        lseek(fd, start, SEEK_SET);
        exec_token_more(root->left, fds, operatorStatus, fd);
    }
    else {            // parent
        wait(0);
        if (operatorStatus & OP_PIPE_W) { 
            int fd2 = open(((t_token*)root->right->data)->value[0], 
                O_RDWR, S_IWRITE | S_IREAD);
            mx_write_from_to(fd2, fds[1], start);
            close(fd2);
        }
    }
    close(fd);
}

static void less_true(t_tnode *root, int *fds, int operatorStatus) {
    int     fd      = 0;
    int     infd      = 0;
    off_t start = 0;
    pid_t pid   = 0;

    fd = setSeek(operatorStatus, (t_token*)root->right->data, &start);
    pid = fork();
    if (pid == 0) {    // child
        infd = open(((t_token*)root->left->right->data)->value[0], O_CREAT, S_IWRITE | S_IREAD);
        dup2(infd, 0);
        lseek(fd, start, SEEK_SET);
        exec_token_more(root->left->left, fds, operatorStatus, fd);
        close(infd);
    }
    else {            // parent
        wait(0);
        if (operatorStatus & OP_PIPE_W) { 
            int fd2 = open(((t_token*)root->right->data)->value[0], 
                O_RDWR, S_IWRITE | S_IREAD);
            mx_write_from_to(fd2, fds[1], start);
            close(fd2);
        }
    }
    close(fd);
}

void mx_exec_more(t_tnode *root, int *fds, int operatorStatus) {
   bool isLess = is_less(root);

    if (isLess == false)
        less_false(root, fds, operatorStatus);
    else if (isLess)
        less_true(root, fds, operatorStatus);
}
