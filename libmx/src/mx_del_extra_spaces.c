#include "libmx.h"

static void write_str(char **temp, char *s, int a) {
    char *s1 = *temp;

    for (int i = 0, b = 0; b < a; i++, b++) {
        while (mx_isspace(s[i]) == 1 && mx_isspace(s[i + 1]) == 1)
            i++;
        if (mx_isspace(s[i]) == 1)
            s1[b] = ' ';
        else
            s1[b] = s[i];
    }
}

char *mx_del_extra_spaces(const char *str) {
    char *s = mx_strtrim(str);
    int a = 0;
    char *s1;

    if (s != 0) {
        a = 0;
        for (int i = 0; s[i]; i++) {
            while (mx_isspace(s[i]) == 1 && mx_isspace(s[i + 1]) == 1)
                i++;
            a++;
        }
        s1 = mx_strnew(a);
        if (s1 != 0)
            write_str(&s1, s, a);
        mx_strdel(&s);
        return s1;
    }
    return s;
}
