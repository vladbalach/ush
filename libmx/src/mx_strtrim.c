#include "libmx.h"

char *mx_strtrim(const char *str) {
    int a;
    int b = 0;
    int d;
    char *c = NULL;

    if (str == 0)
        return 0;
    a = mx_strlen(str);
    while (mx_isspace(str[b]) && str[b] != '\0')
        b++;
    if (b < a)
        while (mx_isspace(str[a - 1]))
            a--;
    d = a - b;
    c = mx_strnew(d);
    if (c != 0)
        for (int i = 0; i < d; i++, b++)
            c[i] = str[b];
    return c;
}
