#include "libmx.h"

void mx_printerr(const char *s) {
    write(2, s, mx_strlen(s));
}

void mx_printcharerr(char c) {
    write(2, &c, 1);
}
