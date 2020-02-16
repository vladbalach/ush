#include "ush.h"

static char **return_read_password(void) {
    int fd = 0;
    char *direc = 0;
    char temp[2];
    char **passwd = 0;

    fd = open("/etc/passwd", O_RDONLY);
    if (fd != -1) {
        temp[1] = 0;
        while (read(fd, temp, 1)) {
            direc = mx_strjoin2(direc, temp);
        }
        close(fd);
    }
    passwd = mx_strsplit(direc, '\n');
    mx_strdel(&direc);
    return passwd;
}

static void read_password(char **user) {
    int index = -1;
    char **passwd = return_read_password();
    char **permisiing = 0;

    while (passwd && passwd[++index]) {
        permisiing = mx_strsplit(passwd[index], ':');
        if (mx_strcmp(permisiing[0], *user) == 0) {
            mx_strdel(user);
            *user = mx_strdup(permisiing[5]);
            mx_del_strarr(&permisiing);
            mx_del_strarr(&passwd);
            return;
        }
        mx_del_strarr(&permisiing);
    }
    mx_del_strarr(&passwd);
    mx_strdel(user);
}

static void read_user(char **user) {
    DIR *dir = opendir("/Users");
    struct dirent *entry;

    if (dir) {
        if (mx_strcmp(*user, "Shared") != 0) { 
            while ((entry = readdir(dir)) != NULL) {
                if (mx_strcmp(entry->d_name, *user) == 0) {
                    mx_strdel(user);
                    *user = mx_strjoin("/Users/", entry->d_name);
                    closedir(dir);
                    return;
                }
            }
        }
        closedir(dir);
    }
    read_password(user);
}

static bool is_not_operator(char c) {
    if (c == '|' || c == '&' || c == '>' || c == '<' || c == ' ')
        return false;
    if (c == 0 || c == '/')
        return false;
    return true;
}

static bool is_not_operator2(char c) {
    if (c == '$' || c == '=' || c == 92 || c == 34 || c == 39 || c == 96)
        return false;
    return true;
}

void mx_HOME(char **str, int *i, t_info *processes) {
    char *temp = *str;
    char *home = 0;
    int new_position = *i;

    if (*i != 0)
        return;
    if (temp[*i + 1] == 0 || temp[*i + 1] == ' ' || temp[*i + 1] == '/') {
        home = mx_strdup("HOME");
        home = mx_strdup(mx_return_value(&home, &processes->var_tree));
        new_position++;
    }
    else if (temp[*i + 1] == '+' && (temp[*i + 2] == 0 || temp[*i + 2] == ' ' || temp[*i + 2] == '/')) {
        home = mx_strdup("PWD");
        home = mx_strdup(mx_return_value(&home, &processes->var_tree));
        new_position += 2;
    }
    else if (temp[*i + 1] == '-' && (temp[*i + 2] == 0 || temp[*i + 2] == ' ' || temp[*i + 2] == '/')) {
        home = mx_strdup("OLDPWD");
        home = mx_strdup(mx_return_value(&home, &processes->var_tree));
        new_position += 2;
    }
    else {
        while (is_not_operator(temp[++new_position]) && is_not_operator2(temp[new_position]));
        if (is_not_operator2(temp[new_position]))
            home = mx_strndup(&temp[*i + 1], new_position - *i - 1);
        if (home) {
            read_user(&home);
        }
    }
    if (home) {
        mx_do_replace(str, *i, new_position, home);
        *i += mx_strlen(home);
        mx_strdel(&home);
    }
}
