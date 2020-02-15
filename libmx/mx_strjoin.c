#include "libmx.h"

char *mx_strjoin(const char *s1, const char *s2) {
    char *s = NULL;
    int a;

    if (s1 != 0 && s2 != 0) {
        s = mx_strnew(mx_strlen(s1) + mx_strlen(s2));
        a = mx_strlen(s1);
        if (s != 0) {
            for (int i = 0 ; i < a; i++)
                s[i] = s1[i];
            for (int i = 0; i < mx_strlen(s2); a++, i++)
                s[a] = s2[i];
        }
        return s;
    }
    if (s1 == 0 && s2 == 0)
        return 0;
    if (s1 != 0)
        return mx_strdup(s1);
    return mx_strdup(s2);
}
