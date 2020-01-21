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
                printf("TRUE\n");
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
        mx_printerr(strerror(errno));
        mx_printerr(" ");
        mx_printerr(token->value[0]);
        mx_printerr("\n");
    }
    mx_del_strarr(&(token->value));
    free(name);
}

void mx_execute_tree(t_tnode *root) {
    int fds[2];
    char *value = ((t_token*)root->data)->value[0];
    pid_t pid = 0;
    
    if (mx_strcmp(value, "|") == 0) {
        pipe(fds);
        pid = fork();
        if (pid == 0) {
            close(1);
            dup(fds[1]);
            char *dir = mx_return_dirname(((t_token*)(root->left->data))->value[0]);
            execute_proces((t_token*)root->left->data, dir);
        }
        else {
            wait(0);
            pid = fork();
            if (pid == 0) {
                close(0);
                dup(fds[0]);
                char *dir = mx_return_dirname(((t_token*)root->right->data)->value[0]);
                execute_proces((t_token*)root->right->data, dir);
            }
            else 
                wait(0);
        }
            
    }
    else{
        char *dirname = mx_return_dirname(((t_token*)root->data)->value[0]);
        pid_t pid = 0;

        pid = fork();
        if (pid == 0)
            execute_proces((t_token*)root->data, dirname);
        else
            wait(0);
        free(dirname);
    }
}
