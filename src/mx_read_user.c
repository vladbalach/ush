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

void mx_read_user(char **user) {
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
