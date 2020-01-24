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



void execute_proces(t_token* token) {
    char **argv = 0;
    char *newArg = 0;
    int i = 0;
    int status = 0;

    while (token->value[i]) {
        mx_add_to_strarr(&argv, token->value[i]);
        i++;
    }
    mx_add_to_strarr(&argv, token->value[i]);
    if (execvp(argv[0], argv) == -1) {
        mx_printerr("u$h: command not found: ");
        mx_printerr(token->value[0]);
        mx_printerr("\n");
        mx_del_strarr(&(token->value));
        exit(1);
    }
}

static void exec_token(t_token* token, int *fds, char pipe_status) {
   
    pid_t pid = fork();

        if (pid == -1) {        // error
            mx_printerr("fork error: ");
            mx_printerr(strerror(errno));
            return;
        }
        else if (pid == 0) {    // child
            if (pipe_status & OP_PIPE_R)
                dup2(fds[0], 0);
            if (pipe_status & OP_PIPE_W)
                dup2(fds[1], 1);
            execute_proces(token);
        }
        else {                  // parent
            wait(0);
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

void mx_execute_tree(t_tnode *root, int *fds, char operatorStatus) {
    if (root == 0)
        return;
    char *cmd = ((t_token*)root->data)->value[0];
    if (((t_token*)root->data)->type == TYPE_COMMAND) {
        exec_token((t_token*)root->data, fds, operatorStatus);
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
    else {
       printf("ELSE\n"); 
    }

}
