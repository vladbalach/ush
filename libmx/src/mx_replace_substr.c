#include "libmx.h"

static int int_str_sub(const char *str, const char *sub, int a, int b) {
    int temp = 0;

    if (mx_get_substr_index(&str[a], sub) > 0)
        temp = mx_get_substr_index(&str[a], sub);
    else
        temp = b;
    return temp;
}

static int char_to_str(const char *str, const char *sub,
                        const char *replace) {
    int a = mx_count_substr(str, sub);
    int b  = mx_strlen(str) - (mx_strlen(sub) - mx_strlen(replace)) * a;

    return b;
}

char *mx_replace_substr(const char *str,
                        const char *sub, const char *replace) {
    int a[5] = {0, 0, 0, 0, 0};
    char *s = NULL;

    if (str == NULL || sub == NULL || replace == NULL)
        return NULL;
    a[1] = char_to_str(str, sub, replace);
    s = mx_strnew(a[1]);
    while (a[2] < a[1]) { 
        while (mx_get_substr_index(&str[a[3]], sub))
            s[a[2]++] = str[a[3]++];
        if (mx_get_substr_index(&str[a[3]], sub) >= 0) {
            for (int l = 0; l < mx_strlen(replace); l++, a[2]++)
                s[a[2]] = replace[l];
            a[3] = a[3] + mx_strlen(sub);
        }
        a[4] = int_str_sub(str, sub, a[3], a[1]);
        for (int k = 0; k < a[4] && a[2] < a[1]; a[2]++, a[3]++, k++)
            s[a[2]] = str[a[3]];
    }
    return s;
}
