#include "libmx.h"

char *mx_strcat(char *restrict s1, const char *restrict s2) {
    int a = 0;

    while (s1[a])
        a++;
    for (int i = 0; s2[i]; i++, a++)
        s1[a] = s2[i];
    s1[a] = '\0';
    return s1;
}
