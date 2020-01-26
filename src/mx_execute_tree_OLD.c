#include "ush.h"


bool check_dir(char *dirname, char *filename) {
    DIR *dir = 0;
    struct dirent *dirnode = 0;

    if ((dirname == 0) || (filename == 0)) {
        return false;
    }
    dir = opendir(dirname);
    if (dir == 0)
        return false;
    else {
        while ((dirnode = readdir(dir)))
            if (mx_strcmp(dirnode->d_name, filename) == 0) {
                closedir(dir);
                return true;
            }
    }
    closedir(dir);
    return false;
}



void mx_exec_token_(t_token* token, int *fds, char operatorStatus) {
    int fds2[2];
    pipe(fds2);
    pid_t pid = fork();

        if (pid == -1) {        // error
            mx_printerr("fork error: ");
            mx_printerr(strerror(errno));
            return;
        }
        else if (pid == 0) {    // child
            if (operatorStatus & OP_PIPE_R){
                 dup2(fds[0], 0);
            }
           
        if (operatorStatus & OP_PIPE_W)
            dup2(fds[1], 1);
        close(fds[1]);
        close(fds[0]);
            mx_execute_proces(token);
        }
        else {                  // parent
            close(fds[1]);
            printf("START2!\n");
            wait(0);
            printf("DONE2!\n");
        }
}

void mx_pipe_execute(t_tnode *root, int *fds, char operatorStatus) {
    if (operatorStatus & OP_PIPE_R) { // if pipe and status == read
        operatorStatus &= 252; // zeroed pipes
        mx_execute_tree(root->left, fds, operatorStatus | OP_PIPE_RW);
        mx_execute_tree(root->right, fds, operatorStatus | OP_PIPE_R);
    }
    else if ((operatorStatus & 252) == 0) { // If no pipes
        operatorStatus &= 252;
        mx_execute_tree(root->left, fds, operatorStatus | OP_PIPE_W);
        mx_execute_tree(root->right, fds, operatorStatus | OP_PIPE_R);
    }
}

// static void mx_pipe_execute_(t_tnode *root, int *fds, char operatorStatus) {
//     int fds2[2];
//     pipe(fds2);
//     pid_t pid = fork();
//     if (pid == 0) {
//         dup2(fds2[1], 1);
//         execvp(((t_token*)root->left->data)->value[0], ((t_token*)root->left->data)->value);
//     }
//     else {
//         wait(0);
//         pid = fork();
//         if (pid == 0) { 
//             dup2(fds2[0], 0);
//             execvp(((t_token*)root->right->data)->value[0], ((t_token*)root->right->data)->value);
//         }
//     }
//     wait(0);
// }

void mx_execute_tree(t_tnode *root, int *fds, char operatorStatus) {
    if (root == 0)
        return;
        printf("TREE\n");
    char *cmd = ((t_token*)root->data)->value[0];
    if (((t_token*)root->data)->type == TYPE_COMMAND) {
        mx_exec_token_((t_token*)root->data, fds, operatorStatus);
    }
    else if (mx_strcmp(((t_token*)root->data)->value[0], "|") == 0) {
        mx_pipe_execute(root, fds, operatorStatus);
    }
    else if (mx_strcmp(((t_token*)root->data)->value[0], ">") == 0) {
        mx_exec_more(root, fds, operatorStatus | OP_MORE);
    }
    else if (mx_strcmp(((t_token*)root->data)->value[0], ">>") == 0) {
        mx_exec_more(root, fds, operatorStatus | OP_DMORE);
    }
    else if (mx_strcmp(((t_token*)root->data)->value[0], "<") == 0) {
        mx_exec_less(root, fds, operatorStatus | OP_LESS);
    }
    else {
       printf("ELSE\n"); 
    }

}
