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

/* if no file return 0*/
char* mx_return_dirname(char *name) {
    if (check_dir("/bin/", name))
        return mx_strdup("/bin/");
    else if(check_dir("/usr/local/bin/", name))
        return mx_strdup("/usr/local/bin/");
        else if(check_dir("/usr/bin/", name))
        return mx_strdup("/usr/bin/");
    else if(check_dir("/usr/sbin/", name))
        return mx_strdup("/usr/sbin/");
    else if(check_dir("/sbin/", name))
        return mx_strdup("/sbin/");
    else if(check_dir("/usr/local/munki/", name))
        return mx_strdup("/usr/local/munki/");
    else 
        return 0;
}

void execute_proces(t_token* token, char *from_dir) {
    char *name = mx_strjoin(from_dir,token->value[0]);
    char **argv = 0;
    char *newArg = 0;
    int i = 1;
    int status = 0;

    mx_add_to_strarr(&argv, name);
    while (token->value[i]) {
        mx_add_to_strarr(&argv, token->value[i]);
        i++;
    }
    if (execv(name,argv) == -1) {
        mx_printerr("Execute error: ");
        mx_printerr(strerror(errno));
        mx_printerr(" ");
        mx_printerr(token->value[0]);
        mx_printerr("\n");
        mx_del_strarr(&(token->value));
        free(name);
        exit(1);
    }
}

static void exec_token(t_token* token, int *fds, char pipe_status) {
    char *dirname = mx_return_dirname(token->value[0]);
    pid_t pid = fork();

        if (pid == -1) {        // error
            mx_printerr("fork error: ");
            mx_printerr(strerror(errno));
            return;
        }
        else if (pid == 0) {    // child
            if (pipe_status & PIPE_R)
                dup2(fds[0], 0);
            if (pipe_status & PIPE_W)
                dup2(fds[1], 1);
            execute_proces(token, dirname);
        }
        else {                  // parent
            free(dirname);
            wait(0);
        }
}

void mx_pipe_execute(t_tnode *root, int *fds, char pipe_status) {
    if (pipe_status == PIPE_R) { // if pipe and status == read
        mx_execute_tree(root->left, fds, PIPE_RW);
        mx_execute_tree(root->right, fds, PIPE_R);
    }
    else if (pipe_status == PIPE_NOTHING) {
        mx_execute_tree(root->left, fds, PIPE_W);
        mx_execute_tree(root->right, fds, PIPE_R);
    }
}

void mx_execute_tree(t_tnode *root, int *fds, char pipeStatus) {
    if (root == 0)
        return;
    char *cmd = ((t_token*)root->data)->value[0];
    if (((t_token*)root->data)->type == TYPE_COMMAND) {
        exec_token((t_token*)root->data, fds, pipeStatus);
    }
    else if (mx_strcmp(((t_token*)root->data)->value[0], "|") == 0) {
        mx_pipe_execute(root, fds, pipeStatus);
    }
    else if (mx_strcmp(((t_token*)root->data)->value[0], ">") == 0) {
        mx_pipe_execute(root, fds, pipeStatus);
    }
    else {
       printf("ELSE\n"); 
    }

}
