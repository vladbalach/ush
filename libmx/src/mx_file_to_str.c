#include "libmx.h"

char *mx_file_to_str(const char *file) {
    int fb;
    int a;
    char *s = NULL;
    char c = '\0';

    if ((fb = open(file, O_RDONLY)) == - 1)
        return 0;
    while ((a = read(fb, &c, 1))) {
        if (a == - 1)
            return 0;
        s = mx_strjoin2(s, &c);
    }
    close(fb);
    return s;
}
