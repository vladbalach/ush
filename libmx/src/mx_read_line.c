#include "libmx.h"

int mx_read_line(char **lineptr, int buf_size, char delim, const int fd) {
    char *s = NULL;
    char a[2] = "";
    int i = 0;

    if (fd == - 1)
        return - 1;
    s = mx_strnew(0);
    *lineptr = s;
    buf_size++;
    while (read(fd, &a[0], 1)) {
        if (a[0] == delim)
            return i;
        s = mx_strjoin2(s,a);
        *lineptr = s;
        i++;
    }
    return 0;
}
