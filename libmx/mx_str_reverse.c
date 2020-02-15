#include "libmx.h"

void mx_str_reverse(char *s) {
    int j = 0;
    char a = '\0';

    if (s != 0) {
        j = mx_strlen(s) - 1;
        for (int i = 0; i < j; i++, j--) {
            a = s[i];
            s[i] = s[j];
            s[j] = a;
        }
    }
}
